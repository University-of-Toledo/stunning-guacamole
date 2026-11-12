/*
Author: Ozzy Finnegan
Lab 12: Blink
Description: Blink an LED using Raspberry Pi GPIO17.
*/

#include <stdio.h>
#include <gpiod.h>
#include <time.h>

#define GPIO_CHIP "/dev/gpiochip0"
#define LED_PIN 17

int main(void)
{
    struct gpiod_chip *chip;
    struct gpiod_line_settings *settings;
    struct gpiod_line_config *line_config;
    struct gpiod_line_request *request; 
    unsigned int offset = LED_PIN;
    struct timespec delay = {1, 0};

    chip = gpiod_chip_open(GPIO_CHIP);

    if (chip == NULL)
    {
        perror("Failed to open GPIO chip");
        return 1;
    }

    settings = gpiod_line_settings_new();

    if (settings == NULL)
    {
        perror("Failed to create line settings");
        gpiod_chip_close(chip);
        return 1;
    }

    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
    gpiod_line_settings_set_output_value(settings, GPIOD_LINE_VALUE_INACTIVE);

    line_config = gpiod_line_config_new();

    if (line_config == NULL)
    {
        perror("Failed to create line configuration");
        gpiod_line_settings_free(settings);
        gpiod_chip_close(chip);
        return 1;
    }

    if (gpiod_line_config_add_line_settings(line_config, &offset, 1, settings) < 0)
    {
        perror("Failed to configure GPIO17");
        gpiod_line_config_free(line_config);
        gpiod_line_settings_free(settings);
        gpiod_chip_close(chip);
        return 1;
    }
    request = gpiod_chip_request_lines(chip, NULL, line_config);

    if (request == NULL)
{
    perror("Failed to request GPIO17");
    gpiod_line_config_free(line_config);
    gpiod_line_settings_free(settings);
    gpiod_chip_close(chip);
    return 1;
}
    for (int i = 0; i < 5; i++)
{
    // Turn LED ON
    if (gpiod_line_request_set_value(request, LED_PIN,
                                    GPIOD_LINE_VALUE_ACTIVE) < 0)
    {
        perror("Failed to turn LED on");
        break;
    }

    printf("LED ON\n");
    nanosleep(&delay, NULL);

    // Turn LED OFF
    if (gpiod_line_request_set_value(request, LED_PIN,
                                    GPIOD_LINE_VALUE_INACTIVE) < 0)
    {
        perror("Failed to turn LED off");
        break;
    }

    printf("LED OFF\n");
    nanosleep(&delay, NULL);
}
    if (gpiod_line_request_set_value(request, LED_PIN,
                                GPIOD_LINE_VALUE_INACTIVE) < 0)
{
    perror("Failed to turn LED off during cleanup");
}
    gpiod_line_request_release(request);
    gpiod_line_config_free(line_config);
    gpiod_line_settings_free(settings);
    gpiod_chip_close(chip);

    return 0;
}   
