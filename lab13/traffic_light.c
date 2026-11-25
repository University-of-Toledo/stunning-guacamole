/*
 Author: Fiza Shaikh
 Exercise: Lab 13: Traffic Light
 Description: Controls red, yellow, and green LEDs on a Raspberry Pi 
 */

#define _DEFAULT_SOURCE
#include <gpiod.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CHIP_PATH "/dev/gpiochip0"
#define NUM_LEDS  3

struct State {
    const char *name;
    int red;
    int yellow;
    int green;
    unsigned int duration_ms;
};

volatile sig_atomic_t running = 1;

void handle_sigint(int sig) {
    (void)sig;
    running = 0;
}

int main(void) {
    unsigned int pins[NUM_LEDS] = {17, 27, 22};

    struct State table[] = {
        { "RED",    1, 0, 0, 5000 },
        { "GREEN",  0, 0, 1, 5000 },
        { "YELLOW", 0, 1, 0, 2000 }
    };
    int num_states = sizeof(table) / sizeof(table[0]);

    enum gpiod_line_value off_values[NUM_LEDS] = {
        GPIOD_LINE_VALUE_INACTIVE,
        GPIOD_LINE_VALUE_INACTIVE,
        GPIOD_LINE_VALUE_INACTIVE
    };

    if (signal(SIGINT, handle_sigint) == SIG_ERR) {
        perror("signal");
        return 1;
    }

    struct gpiod_chip *chip = gpiod_chip_open(CHIP_PATH);
    if (!chip) {
        perror("gpiod_chip_open");
        return 1;
    }

    struct gpiod_line_settings *settings = gpiod_line_settings_new();
    if (!settings) {
        perror("gpiod_line_settings_new");
        gpiod_chip_close(chip);
        return 1;
    }
    if (gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT) < 0 ||
        gpiod_line_settings_set_output_value(settings, GPIOD_LINE_VALUE_INACTIVE) < 0) {
        perror("gpiod_line_settings");
        gpiod_line_settings_free(settings);
        gpiod_chip_close(chip);
        return 1;
    }

    struct gpiod_line_config *line_cfg = gpiod_line_config_new();
    if (!line_cfg) {
        perror("gpiod_line_config_new");
        gpiod_line_settings_free(settings);
        gpiod_chip_close(chip);
        return 1;
    }
    if (gpiod_line_config_add_line_settings(line_cfg, pins, NUM_LEDS, settings) < 0) {
        perror("gpiod_line_config_add_line_settings");
        gpiod_line_config_free(line_cfg);
        gpiod_line_settings_free(settings);
        gpiod_chip_close(chip);
        return 1;
    }

    struct gpiod_request_config *req_cfg = gpiod_request_config_new();
    if (!req_cfg) {
        perror("gpiod_request_config_new");
        gpiod_line_config_free(line_cfg);
        gpiod_line_settings_free(settings);
        gpiod_chip_close(chip);
        return 1;
    }
    gpiod_request_config_set_consumer(req_cfg, "traffic_light");

    struct gpiod_line_request *request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);
    if (!request) {
        perror("gpiod_chip_request_lines");
        gpiod_request_config_free(req_cfg);
        gpiod_line_config_free(line_cfg);
        gpiod_line_settings_free(settings);
        gpiod_chip_close(chip);
        return 1;
    }

    if (gpiod_line_request_set_values(request, off_values) < 0) {
        perror("gpiod_line_request_set_values");
        running = 0;
    }

    printf("Traffic light running. Press Ctrl+C to stop.\n");

    int i = 0;
    while (running) {
        enum gpiod_line_value current_values[NUM_LEDS] = {
            table[i].red    ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE,
            table[i].yellow ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE,
            table[i].green  ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE
        };

        printf("State: %s (%u ms)\n", table[i].name, table[i].duration_ms);

        if (gpiod_line_request_set_values(request, current_values) < 0) {
            perror("gpiod_line_request_set_values");
            break;
        }

        usleep(table[i].duration_ms * 1000);
        i = (i + 1) % num_states;
    }

    printf("\nTurning off LEDs and exiting...\n");
    if (gpiod_line_request_set_values(request, off_values) < 0) {
        perror("gpiod_line_request_set_values");
    }

    gpiod_line_request_release(request);
    gpiod_request_config_free(req_cfg);
    gpiod_line_config_free(line_cfg);
    gpiod_line_settings_free(settings);
    gpiod_chip_close(chip);

    return 0;
}
