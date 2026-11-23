
/*
Author: Ozzy Finnegan
EET 3150 - Bitwise Operators and GPIO
Lab: 5-LED Traffic Light

Raspberry Pi 5 / libgpiod v2

LED connections:
RED       GPIO17 - Physical Pin 11
YELLOW    GPIO27 - Physical Pin 13
GREEN     GPIO22 - Physical Pin 15
Y_ARROW   GPIO23 - Physical Pin 16
G_ARROW   GPIO24 - Physical Pin 18

*/

#include <stdio.h>
#include <unistd.h>
#include <gpiod.h>

#define NUM_LEDS 5
#define GPIO_CHIP "/dev/gpiochip0"

#define RED       (1u << 0)
#define YELLOW    (1u << 1)
#define GREEN     (1u << 2)
#define Y_ARROW   (1u << 3)
#define G_ARROW   (1u << 4)

#define ARROWS    (Y_ARROW | G_ARROW)

static const unsigned int led_pins[NUM_LEDS] =
{
    17, 27, 22, 23, 24
};

struct gpiod_line_request *gpio_setup(void);

int set_lights(struct gpiod_line_request *request,
               unsigned int signal);

int show(struct gpiod_line_request *request,
         const char *step,
         unsigned int signal,
         unsigned int seconds);


int main(void)
{
    struct gpiod_line_request *request;
    unsigned int signal;
    int cycle;
    int flash;
    int result = 0;

    request = gpio_setup();

    if (request == NULL)
    {
        fprintf(stderr, "GPIO setup failed.\n");
        return 1;
    }

    printf("5-LED Bitwise Traffic Light\n");
    printf("---------------------------\n");

    signal = RED;

    for (cycle = 0; cycle < 2; cycle++)
    {
        printf("\nCycle %d\n", cycle + 1);

        if (show(request, "Red", signal, 2) != 0)
        {
            result = 1;
            break;
        }

        signal = signal | G_ARROW;

        if (show(request, "Red + Green Arrow", signal, 3) != 0)
        {
            result = 1;
            break;
        }

        signal = (signal & ~ARROWS) | Y_ARROW;

        if (show(request, "Red + Yellow Arrow", signal, 2) != 0)
        {
            result = 1;
            break;
        }

        signal = signal & ~Y_ARROW;

        if (show(request, "Red Only", signal, 1) != 0)
        {
            result = 1;
            break;
        }

        /*
        Modification #3:
        Rewrite Step 5 without using XOR.
        */
        signal = (signal & ~RED) | GREEN;

        if (show(request, "Green", signal, 3) != 0)
        {
            result = 1;
            break;
        }

        signal = signal ^ (GREEN | YELLOW);

        if (show(request, "Yellow", signal, 2) != 0)
        {
            result = 1;
            break;
        }

        signal = signal ^ (YELLOW | RED);

        if (show(request, "Red", signal, 1) != 0)
        {
            result = 1;
            break;
        }
    }

    /*
    Modification #1:
    Flashing Yellow Arrow.    
    */
    if (result == 0)
    {
        printf("\nFlashing Yellow Arrow\n");

        signal = RED;

        for (flash = 0; flash < 5; flash++)
        {
            signal = signal ^ Y_ARROW;

            if (show(request, "Red + Yellow Arrow", signal, 1) != 0)
            {
                result = 1;
                break;
            }

            signal = signal ^ Y_ARROW;

            if (show(request, "Red Only", signal, 1) != 0)
            {
                result = 1;
                break;
            }
        }
    }

    if (set_lights(request, 0u) != 0)
    {
        fprintf(stderr, "Failed to turn off LEDs.\n");
        result = 1;
    }

    gpiod_line_request_release(request);

    printf("\nTraffic light finished.\n");

    return result;
}
struct gpiod_line_request *gpio_setup(void)
{
    struct gpiod_chip *chip = NULL;
    struct gpiod_line_settings *settings = NULL;
    struct gpiod_line_config *config = NULL;
    struct gpiod_line_request *request = NULL;

    chip = gpiod_chip_open(GPIO_CHIP);

    if (chip == NULL)
    {
        perror("gpiod_chip_open");
        goto cleanup;
    }

    settings = gpiod_line_settings_new();
    config = gpiod_line_config_new();

    if (settings == NULL || config == NULL)
    {
        fprintf(stderr, "GPIO allocation failed.\n");
        goto cleanup;
    }

    if (gpiod_line_settings_set_direction(
            settings,
            GPIOD_LINE_DIRECTION_OUTPUT) != 0)
    {
        fprintf(stderr, "GPIO direction failed.\n");
        goto cleanup;
    }

    if (gpiod_line_settings_set_output_value(
            settings,
            GPIOD_LINE_VALUE_INACTIVE) != 0)
    {
        fprintf(stderr, "GPIO initial value failed.\n");
        goto cleanup;
    }

    if (gpiod_line_config_add_line_settings(
            config,
            led_pins,
            NUM_LEDS,
            settings) != 0)
    {
        fprintf(stderr, "GPIO line configuration failed.\n");
        goto cleanup;
    }

    request = gpiod_chip_request_lines(
        chip,
        NULL,
        config);

    if (request == NULL)
    {
        perror("gpiod_chip_request_lines");
    }

cleanup:
    gpiod_line_config_free(config);
    gpiod_line_settings_free(settings);
    gpiod_chip_close(chip);

    return request;
}


int set_lights(struct gpiod_line_request *request,
               unsigned int signal)
{
    unsigned int bit;

    for (int i = 0; i < NUM_LEDS; i++)
    {
        if (gpiod_line_request_set_value(
                request,
                led_pins[i],
                GPIOD_LINE_VALUE_INACTIVE) != 0)
        {
            perror("GPIO clear");
            return -1;
        }
    }

    for (int i = 0; i < NUM_LEDS; i++)
    {
        bit = (signal >> i) & 1u;

        if (bit == 1u)
        {
            if (gpiod_line_request_set_value(
                    request,
                    led_pins[i],
                    GPIOD_LINE_VALUE_ACTIVE) != 0)
            {
                perror("GPIO set");
                return -1;
            }
        }
    }

    return 0;
}

int show(struct gpiod_line_request *request,
         const char *step,
         unsigned int signal,
         unsigned int seconds)
{
    unsigned int bit;

    printf("%-25s ", step);

    for (int i = NUM_LEDS - 1; i >= 0; i--)
    {
        bit = (signal >> i) & 1u;
        printf("%u", bit);
    }

    printf("\n");
    fflush(stdout);

    if (set_lights(request, signal) != 0)
    {
        return -1;
    }

    sleep(seconds);

    return 0;
}
