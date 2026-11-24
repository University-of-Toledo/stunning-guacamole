
/*
Author: Ozzy Finnegan
Course: EET 3150 - C Programming
Lab 14: Button with Debounce
Description: Reads an active-low push-button on GPIO23
             and controls an LED on GPIO17.
*/

#include <errno.h>
#include <gpiod.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GPIO_CHIP "/dev/gpiochip0"
#define LED_PIN 17u
#define BUTTON_PIN 23u
#define DEBOUNCE_MS 30L
#define POLL_MS 5L

static volatile sig_atomic_t keep_running = 1;


static void handle_signal(int signal_number);
static int get_time(struct timespec *timestamp);
static long elapsed_ms(const struct timespec *start,
                       const struct timespec *end);
static int sleep_ms(long milliseconds);
static int set_led(struct gpiod_line_request *request, int on);
static int run_button_loop(struct gpiod_line_request *button_request,
                           struct gpiod_line_request *led_request);

static void handle_signal(int signal_number)
{
    (void)signal_number;
    keep_running = 0;
}

static int get_time(struct timespec *timestamp)
{
    if (clock_gettime(CLOCK_MONOTONIC, timestamp) != 0)
    {
        perror("clock_gettime");
        return -1;
    }

    return 0;
}

static long elapsed_ms(const struct timespec *start,
                       const struct timespec *end)
{
    long seconds = (long)(end->tv_sec - start->tv_sec);
    long nanoseconds = end->tv_nsec - start->tv_nsec;

    return seconds * 1000L + nanoseconds / 1000000L;
}

static int sleep_ms(long milliseconds)
{
    struct timespec duration;

    duration.tv_sec = milliseconds / 1000L;
    duration.tv_nsec = (milliseconds % 1000L) * 1000000L;

    if (nanosleep(&duration, NULL) != 0 && errno != EINTR)
    {
        perror("nanosleep");
        return -1;
    }

    return 0;
}

static int set_led(struct gpiod_line_request *request, int on)
{
    enum gpiod_line_value value;

    value = on ? GPIOD_LINE_VALUE_ACTIVE
               : GPIOD_LINE_VALUE_INACTIVE;

    if (gpiod_line_request_set_value(request, LED_PIN, value) != 0)
    {
        perror("set_led");
        return -1;
    }

    return 0;
}

static int run_button_loop(struct gpiod_line_request *button_request,
                           struct gpiod_line_request *led_request)
{
    enum gpiod_line_value raw_value;
    int raw_pressed;
    int last_raw_pressed;
    int stable_pressed;
    struct timespec last_change;
    struct timespec now;

    raw_value = gpiod_line_request_get_value(button_request, BUTTON_PIN);

    if (raw_value == GPIOD_LINE_VALUE_ERROR)
    {
        fprintf(stderr, "Failed to read button.\n");
        return -1;
    }

    last_raw_pressed = (raw_value == GPIOD_LINE_VALUE_INACTIVE);
    stable_pressed = last_raw_pressed;

    if (get_time(&last_change) != 0)
    {
        return -1;
    }

    if (set_led(led_request, stable_pressed) != 0)
    {
        return -1;
    }

    printf("Button debounce running. Press Ctrl+C to exit.\n");

    while (keep_running)
    {
        raw_value = gpiod_line_request_get_value(button_request, BUTTON_PIN);

        if (raw_value == GPIOD_LINE_VALUE_ERROR)
        {
            fprintf(stderr, "Failed to read button.\n");
            return -1;
        }

        raw_pressed = (raw_value == GPIOD_LINE_VALUE_INACTIVE);

        if (get_time(&now) != 0)
        {
            return -1;
        }

        if (raw_pressed != last_raw_pressed)
        {
            last_raw_pressed = raw_pressed;
            last_change = now;
        }

        if (raw_pressed != stable_pressed &&
            elapsed_ms(&last_change, &now) >= DEBOUNCE_MS)
        {
            stable_pressed = raw_pressed;

            if (set_led(led_request, stable_pressed) != 0)
            {
                return -1;
            }

            printf("Button %s - LED %s\n",
                   stable_pressed ? "PRESSED" : "RELEASED",
                   stable_pressed ? "ON" : "OFF");
            fflush(stdout);
        }

        if (sleep_ms(POLL_MS) != 0)
        {
            return -1;
        }
    }

    return 0;
}

int main(void)
{
    struct gpiod_chip *chip = NULL;
    struct gpiod_line_settings *button_settings = NULL;
    struct gpiod_line_settings *led_settings = NULL;
    struct gpiod_line_config *button_config = NULL;
    struct gpiod_line_config *led_config = NULL;
    struct gpiod_line_request *button_request = NULL;
    struct gpiod_line_request *led_request = NULL;
    unsigned int button_pin = BUTTON_PIN;
    unsigned int led_pin = LED_PIN;
    int result = EXIT_FAILURE;

    if (signal(SIGINT, handle_signal) == SIG_ERR)
    {
        perror("signal");
        return EXIT_FAILURE;
    }

    chip = gpiod_chip_open(GPIO_CHIP);

    if (chip == NULL)
    {
        perror("gpiod_chip_open");
        goto cleanup;
    }

    button_settings = gpiod_line_settings_new();
    led_settings = gpiod_line_settings_new();
    button_config = gpiod_line_config_new();
    led_config = gpiod_line_config_new();

    if (button_settings == NULL || led_settings == NULL ||
        button_config == NULL || led_config == NULL)
    {
        fprintf(stderr, "GPIO allocation failed.\n");
        goto cleanup;
    }

    if (gpiod_line_settings_set_direction(
            button_settings, GPIOD_LINE_DIRECTION_INPUT) != 0)
    {
        fprintf(stderr, "Button direction failed.\n");
        goto cleanup;
    }

    if (gpiod_line_settings_set_bias(
            button_settings, GPIOD_LINE_BIAS_PULL_UP) != 0)
    {
        fprintf(stderr, "Button pull-up failed.\n");
        goto cleanup;
    }

    if (gpiod_line_settings_set_direction(
            led_settings, GPIOD_LINE_DIRECTION_OUTPUT) != 0)
    {
        fprintf(stderr, "LED direction failed.\n");
        goto cleanup;
    }

    if (gpiod_line_settings_set_output_value(
            led_settings, GPIOD_LINE_VALUE_INACTIVE) != 0)
    {
        fprintf(stderr, "LED initial state failed.\n");
        goto cleanup;
    }

    if (gpiod_line_config_add_line_settings(
            button_config, &button_pin, 1, button_settings) != 0 ||
        gpiod_line_config_add_line_settings(
            led_config, &led_pin, 1, led_settings) != 0)
    {
        fprintf(stderr, "GPIO configuration failed.\n");
        goto cleanup;
    }

    button_request = gpiod_chip_request_lines(chip, NULL, button_config);

    if (button_request == NULL)
    {
        perror("button request");
        goto cleanup;
    }

    led_request = gpiod_chip_request_lines(chip, NULL, led_config);

    if (led_request == NULL)
    {
        perror("LED request");
        goto cleanup;
    }

    result = run_button_loop(button_request, led_request) == 0
             ? EXIT_SUCCESS : EXIT_FAILURE;

cleanup:
    if (led_request != NULL)
    {
        if (set_led(led_request, 0) != 0)
        {
            result = EXIT_FAILURE;
        }

        gpiod_line_request_release(led_request);
    }

    if (button_request != NULL)
    {
        gpiod_line_request_release(button_request);
    }

    gpiod_line_config_free(led_config);
    gpiod_line_config_free(button_config);
    gpiod_line_settings_free(led_settings);
    gpiod_line_settings_free(button_settings);

    if (chip != NULL)
    {
        gpiod_chip_close(chip);
    }

    return result;
}
