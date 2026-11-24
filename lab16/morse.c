
/*
Author: Ozzy Finnegan
Course: EET 3150 - C Programming
Lab 16: Morse Code Blinker
Description: Transmits text as Morse code using
*/

#include <ctype.h>
#include <errno.h>
#include <gpiod.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

#define GPIO_CHIP "/dev/gpiochip0"
#define LED_PIN 17
#define DEFAULT_UNIT_MS 200
static volatile sig_atomic_t stop_requested = 0;


static void handle_signal(int signal_number)
{
    (void)signal_number;
    stop_requested = 1;
}

struct MorseEntry
{
    char symbol;
    const char *pattern;
};

static const struct MorseEntry morse_table[] =
{
    {'A', ".-"},    {'B', "-..."},  {'C', "-.-."},
    {'D', "-.."},   {'E', "."},     {'F', "..-."},
    {'G', "--."},   {'H', "...."},  {'I', ".."},
    {'J', ".---"},  {'K', "-.-"},   {'L', ".-.."},
    {'M', "--"},    {'N', "-."},    {'O', "---"},
    {'P', ".--."},  {'Q', "--.-"},  {'R', ".-."},
    {'S', "..."},   {'T', "-"},     {'U', "..-"},
    {'V', "...-"},  {'W', ".--"},  {'X', "-..-"},
    {'Y', "-.--"},  {'Z', "--.."},
    {'0', "-----"}, {'1', ".----"}, {'2', "..---"},
    {'3', "...--"}, {'4', "....-"}, {'5', "....."},
    {'6', "-...."}, {'7', "--..."}, {'8', "---.."},
    {'9', "----."}
};

static const char *find_pattern(char character)
{
    size_t count = sizeof(morse_table) / sizeof(morse_table[0]);

    character = (char)toupper((unsigned char)character);

    for (size_t i = 0; i < count; i++)
    {
        if (morse_table[i].symbol == character)
        {
            return morse_table[i].pattern;
        }
    }

    return NULL;
}


static int delay_units(unsigned int units, unsigned int unit_ms)
{
    unsigned long long total_ms =
        (unsigned long long)units * unit_ms;

    struct timespec remaining;

    remaining.tv_sec = (time_t)(total_ms / 1000u);
    remaining.tv_nsec = (long)(total_ms % 1000u) * 1000000L;

    if (stop_requested)
    {
        return -1;
    }

    while (nanosleep(&remaining, &remaining) != 0)
    {
        if (errno == EINTR)
        {
            if (stop_requested)
            {
                return -1;
            }

            continue;
        }

        perror("nanosleep");
        return -1;
    }

    return stop_requested ? -1 : 0;
}

static int set_led(struct gpiod_line_request *request, int on)
{
    enum gpiod_line_value value = on
        ? GPIOD_LINE_VALUE_ACTIVE
        : GPIOD_LINE_VALUE_INACTIVE;

    if (gpiod_line_request_set_value(request, LED_PIN, value) != 0)
    {
        perror("gpiod_line_request_set_value");
        return -1;
    }

    return 0;
}

static int transmit_message(struct gpiod_line_request *request,
                            const char *message,
                            unsigned int unit_ms)
{
    int previous_character = 0;
    int word_gap = 0;

    for (size_t i = 0; message[i] != '\0' && !stop_requested; i++)
    {
        unsigned char character = (unsigned char)message[i];

        if (isspace(character))
        {
            if (previous_character)
            {
                word_gap = 1;
            }

            continue;
        }

        const char *pattern = find_pattern((char)character);

        if (pattern == NULL)
        {
            fprintf(stderr, "Warning: unsupported character '%c'\n",
                    character);
            continue;
        }

        if (previous_character)
        {
            if (delay_units(word_gap ? 7u : 3u, unit_ms) != 0)
            {
                return -1;
            }
        }

        printf("%c: %s\n", toupper(character), pattern);
        fflush(stdout);

        for (size_t j = 0; pattern[j] != '\0' && !stop_requested; j++)
        {
            if (j > 0 && delay_units(1u, unit_ms) != 0)
            {
                return -1;
            }

            if (set_led(request, 1) != 0)
            {
                return -1;
            }

            unsigned int duration = pattern[j] == '.' ? 1u : 3u;

            if (delay_units(duration, unit_ms) != 0)
            {
                return -1;
            }

            if (set_led(request, 0) != 0)
            {
                return -1;
            }
        }

        previous_character = 1;
        word_gap = 0;
    }

    return stop_requested ? -1 : 0;
}

int main(int argc, char *argv[])
{
    unsigned int unit_ms = DEFAULT_UNIT_MS;
    struct gpiod_chip *chip = NULL;
    struct gpiod_line_settings *settings = NULL;
    struct gpiod_line_config *config = NULL;
    struct gpiod_line_request *request = NULL;
    int result = EXIT_FAILURE;

    struct sigaction action = {0};

    action.sa_handler = handle_signal;

    if (sigemptyset(&action.sa_mask) != 0)
    {
        perror("sigemptyset");
        return EXIT_FAILURE;
    }

    if (sigaction(SIGINT, &action, NULL) != 0)
    {
        perror("sigaction SIGINT");
        return EXIT_FAILURE;
    }

    if (sigaction(SIGTERM, &action, NULL) != 0)
    {
        perror("sigaction SIGTERM");
        return EXIT_FAILURE;
    }


    if (argc < 2 || argc > 3)
    {
        fprintf(stderr, "Usage: %s \"MESSAGE\" [unit_ms]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (argc == 3)
    {
        char *end = NULL;
        unsigned long parsed;

        errno = 0;
        parsed = strtoul(argv[2], &end, 10);

        if (errno != 0 || end == argv[2] || *end != '\0' ||
            argv[2][0] == '-' || parsed == 0 || parsed > 10000)
        {
            fprintf(stderr, "Invalid unit: enter 1 to 10000 ms.\n");
            return EXIT_FAILURE;
        }

        unit_ms = (unsigned int)parsed;
    }

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
            settings, GPIOD_LINE_DIRECTION_OUTPUT) != 0)
    {
        fprintf(stderr, "GPIO direction setup failed.\n");
        goto cleanup;
    }

    if (gpiod_line_settings_set_output_value(
            settings, GPIOD_LINE_VALUE_INACTIVE) != 0)
    {
        fprintf(stderr, "GPIO initial value failed.\n");
        goto cleanup;
    }

    unsigned int pin = LED_PIN;

    if (gpiod_line_config_add_line_settings(
            config, &pin, 1, settings) != 0)
    {
        fprintf(stderr, "GPIO configuration failed.\n");
        goto cleanup;
    }

    request = gpiod_chip_request_lines(chip, NULL, config);

    if (request == NULL)
    {
        perror("gpiod_chip_request_lines");
        goto cleanup;
    }

    printf("Morse Code Blinker - GPIO17\n");
    printf("Base unit: %u ms\n\n", unit_ms);

    if (transmit_message(request, argv[1], unit_ms) != 0)
    {
        goto cleanup;
    }

    result = EXIT_SUCCESS;

cleanup:
    if (request != NULL)
    {
        if (set_led(request, 0) != 0)
        {
            result = EXIT_FAILURE;
        }

        gpiod_line_request_release(request);
    }

    gpiod_line_config_free(config);
    gpiod_line_settings_free(settings);

    if (chip != NULL)
    {
        gpiod_chip_close(chip);
    }

    return result;
}
