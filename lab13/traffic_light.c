
/*
Author: Ozzy Finnegan
Lab 13: Traffic Light
Description: Control three LEDs using Raspberry Pi GPIO.
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <gpiod.h>

#define GPIO_CHIP "/dev/gpiochip0"

#define RED_PIN 17
#define YELLOW_PIN 27
#define GREEN_PIN 22

#define NUM_LEDS 3

static volatile sig_atomic_t running = 1;

void handle_signal(int signal_number)
{
    (void)signal_number;
    running = 0;
}

struct TrafficState
{
    const char *name;
    enum gpiod_line_value red;
    enum gpiod_line_value yellow;
    enum gpiod_line_value green;
    int duration;
};

void wait_seconds(int seconds)
{
    struct timespec delay = {1, 0};

    for (int i = 0; i < seconds && running; i++)
    {
        if (nanosleep(&delay, NULL) != 0 && running)
        {
            perror("nanosleep");
            running = 0;
        }
    }
}

int set_lights(struct gpiod_line_request *request,
               const struct TrafficState *state)
{
    enum gpiod_line_value values[NUM_LEDS] =
    {
        state->red,
        state->yellow,
        state->green
    };

    unsigned int offsets[NUM_LEDS] =
    {
        RED_PIN,
        YELLOW_PIN,
        GREEN_PIN
    };

    return gpiod_line_request_set_values_subset(
        request, NUM_LEDS, offsets, values);
}

int main(void)
{
    struct gpiod_chip *chip = NULL;
    struct gpiod_line_settings *settings = NULL;
    struct gpiod_line_config *line_config = NULL;
    struct gpiod_request_config *request_config = NULL;
    struct gpiod_line_request *request = NULL;

    unsigned int offsets[NUM_LEDS] =
    {
        RED_PIN,
        YELLOW_PIN,
        GREEN_PIN
    };

    const struct TrafficState states[] =
    {
        {
            "RED",
            GPIOD_LINE_VALUE_ACTIVE,
            GPIOD_LINE_VALUE_INACTIVE,
            GPIOD_LINE_VALUE_INACTIVE,
            5
        },
        {
            "GREEN",
            GPIOD_LINE_VALUE_INACTIVE,
            GPIOD_LINE_VALUE_INACTIVE,
            GPIOD_LINE_VALUE_ACTIVE,
            5
        },
        {
            "YELLOW",
            GPIOD_LINE_VALUE_INACTIVE,
            GPIOD_LINE_VALUE_ACTIVE,
            GPIOD_LINE_VALUE_INACTIVE,
            2
        }
    };

    const int state_count =
        sizeof(states) / sizeof(states[0]);

    int result = EXIT_FAILURE;

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    printf("Traffic Light Program Starting...\n");

    chip = gpiod_chip_open(GPIO_CHIP);

    if (chip == NULL)
    {
        perror("gpiod_chip_open");
        goto cleanup;
    }

    settings = gpiod_line_settings_new();

    if (settings == NULL)
    {
        fprintf(stderr, "Failed to create line settings.\n");
        goto cleanup;
    }

    if (gpiod_line_settings_set_direction(
            settings, GPIOD_LINE_DIRECTION_OUTPUT) != 0)
    {
        fprintf(stderr, "Failed to set output direction.\n");
        goto cleanup;
    }

    gpiod_line_settings_set_output_value(
        settings, GPIOD_LINE_VALUE_INACTIVE);

    line_config = gpiod_line_config_new();

    if (line_config == NULL)
    {
        fprintf(stderr, "Failed to create line configuration.\n");
        goto cleanup;
    }

    if (gpiod_line_config_add_line_settings(
            line_config, offsets, NUM_LEDS, settings) != 0)
    {
        fprintf(stderr, "Failed to configure GPIO lines.\n");
        goto cleanup;
    }

    request_config = gpiod_request_config_new();

    if (request_config == NULL)
    {
        fprintf(stderr, "Failed to create request configuration.\n");
        goto cleanup;
    }

    gpiod_request_config_set_consumer(
        request_config, "EET3150-TrafficLight");

    request = gpiod_chip_request_lines(
        chip, request_config, line_config);

    if (request == NULL)
    {
        perror("gpiod_chip_request_lines");
        goto cleanup;
    }

    printf("GPIO lines configured successfully.\n");
    printf("Press Ctrl+C to stop.\n\n");

    while (running)
    {
        for (int i = 0; i < state_count && running; i++)
        {
            if (set_lights(request, &states[i]) != 0)
            {
                perror("set_lights");
                goto cleanup;
            }

            printf("%s light ON for %d seconds.\n",
                   states[i].name,
                   states[i].duration);

            fflush(stdout);
            wait_seconds(states[i].duration);
        }
    }

    result = EXIT_SUCCESS;

cleanup:

    if (request != NULL)
    {
        const struct TrafficState off =
        {
            "OFF",
            GPIOD_LINE_VALUE_INACTIVE,
            GPIOD_LINE_VALUE_INACTIVE,
            GPIOD_LINE_VALUE_INACTIVE,
            0
        };

        if (set_lights(request, &off) != 0)
        {
            perror("Failed to turn LEDs off");
            result = EXIT_FAILURE;
        }

        gpiod_line_request_release(request);
    }

    if (request_config != NULL)
    {
        gpiod_request_config_free(request_config);
    }

    if (line_config != NULL)
    {
        gpiod_line_config_free(line_config);
    }

    if (settings != NULL)
    {
        gpiod_line_settings_free(settings);
    }

    if (chip != NULL)
    {
        gpiod_chip_close(chip);
    }

    printf("Traffic Light Program Stopped.\n");

    return result;
}
