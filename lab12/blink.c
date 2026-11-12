/**
 Name: Fiza Shaikh
 Lab 12: Blink
 Description: Blinks an LED connected to GPIO17 using libgpiod v2 API
*/ 

#define _POSIX_C_SOURCE 200809L
#include <gpiod.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CHIP_PATH   "/dev/gpiochip0"
#define LINE_OFFSET 17
#define CONSUMER    "lab12-blink"
#define BLINK_COUNT 10

static volatile sig_atomic_t keep_running = 1;

static void handle_sigint(int sig) {
    (void)sig;
    keep_running = 0;
}

int main(void) {
    struct sigaction sa = {0};
    sa.sa_handler = handle_sigint;
    if (sigaction(SIGINT, &sa, NULL) < 0) {
        perror("sigaction");
        return EXIT_FAILURE;
    }

    struct gpiod_chip *chip = gpiod_chip_open(CHIP_PATH);
    if (!chip) {
        perror("gpiod_chip_open");
        return EXIT_FAILURE;
    }

    struct gpiod_line_settings *settings = gpiod_line_settings_new();
    if (!settings) {
        perror("gpiod_line_settings_new");
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    if (gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT) < 0 ||
        gpiod_line_settings_set_output_value(settings, GPIOD_LINE_VALUE_INACTIVE) < 0) {
        perror("gpiod_line_settings");
        gpiod_line_settings_free(settings);
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    struct gpiod_line_config *line_cfg = gpiod_line_config_new();
    if (!line_cfg) {
        perror("gpiod_line_config_new");
        gpiod_line_settings_free(settings);
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    unsigned int offset = LINE_OFFSET;
    if (gpiod_line_config_add_line_settings(line_cfg, &offset, 1, settings) < 0) {
        perror("gpiod_line_config_add_line_settings");
        gpiod_line_config_free(line_cfg);
        gpiod_line_settings_free(settings);
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    struct gpiod_request_config *req_cfg = gpiod_request_config_new();
    if (!req_cfg) {
        perror("gpiod_request_config_new");
        gpiod_line_config_free(line_cfg);
        gpiod_line_settings_free(settings);
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }
    gpiod_request_config_set_consumer(req_cfg, CONSUMER);

    struct gpiod_line_request *request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);

    /* Free configuration objects immediately after requesting the line */
    gpiod_request_config_free(req_cfg);
    gpiod_line_config_free(line_cfg);
    gpiod_line_settings_free(settings);

    if (!request) {
        perror("gpiod_chip_request_lines");
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    struct timespec delay = { .tv_sec = 0, .tv_nsec = 500000000L }; /* 500 ms */
    enum gpiod_line_value val = GPIOD_LINE_VALUE_INACTIVE;
    int status = EXIT_SUCCESS;

    for (int i = 0; i < BLINK_COUNT * 2 && keep_running; i++) {
        val = (val == GPIOD_LINE_VALUE_INACTIVE) ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE;
        if (gpiod_line_request_set_value(request, offset, val) < 0) {
            perror("gpiod_line_request_set_value");
            status = EXIT_FAILURE;
            break;
        }
        if (nanosleep(&delay, NULL) < 0 && keep_running) {
            perror("nanosleep");
            status = EXIT_FAILURE;
            break;
        }
    }

    if (gpiod_line_request_set_value(request, offset, GPIOD_LINE_VALUE_INACTIVE) < 0) {
        perror("gpiod_line_request_set_value (shutdown)");
        status = EXIT_FAILURE;
    }

    gpiod_line_request_release(request);
    gpiod_chip_close(chip);
    return status;
}
