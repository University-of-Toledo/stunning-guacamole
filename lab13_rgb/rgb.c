/*
 * rgb.c
 *
 * Author:      Ozzy Finnegan
 * Original starter code supplied by instructor; modifications marked below.
 * Exercise:    CSET 3150 Lab 13 - Multi-Color LED
 * Description: Drives an RGB LED from three GPIO pins with libgpiod 2.2.1
 *              on a Raspberry Pi 4 or Pi 5. Each channel is fully on or
 *              off, giving eight colors.
 *
 *              Usage:
 *                ./rgb                 cycle through all colors
 *                ./rgb -c <color>      show one color until Ctrl+C
 *                ./rgb -d <ms> -n <N>  cycle with a custom delay, N times
 *                ./rgb -a              common-anode LED (inverts outputs)
 *
 *              Wiring (BCM numbering, physical pin in parentheses):
 *                GPIO17 (pin 11) -> 330 ohm -> red anode
 *                GPIO27 (pin 13) -> 330 ohm -> green anode
 *                GPIO22 (pin 15) -> 330 ohm -> blue anode
 *                GND    (pin 9)  -> common cathode (longest lead)
 *
 *              Added option: -p <chip_path>
 *              Build: gcc -Wall -Wextra -Werror -o rgb rgb.c -lgpiod
 */

#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <gpiod.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <unistd.h>


static const char CONSUMER[] = "lab13-rgb";

enum
{
    NUM_PINS = 3
};
static unsigned int LED_PINS[NUM_PINS] = {17, 27, 22};

static const long DEFAULT_DELAY_MS = 500;
static const long MAX_DELAY_MS = 60000;
static const long MAX_CYCLES = 1000000;




enum
{
    RED = 1 << 0,
    GREEN = 1 << 1,
    BLUE = 1 << 2
};

static const struct color
{
    const char *name;
    int mask;
} COLORS[] = {
    {"off",     0                 },
    {"red",     RED               },
    {"green",   GREEN             },
    {"blue",    BLUE              },
    {"yellow",  RED | GREEN       },
    {"cyan",    GREEN | BLUE      },
    {"magenta", RED | BLUE        },
    {"white",   RED | GREEN | BLUE},
};
enum
{
    NUM_COLORS = sizeof(COLORS) / sizeof(COLORS[0])
};

/* ---- Board selection ----------------------------------------------------- */

// ORIGINAL PROFESSOR CODE (disabled):
// /* Pi 4: the BCM2711 drives the header and is gpiochip0. */
// static struct gpiod_chip *open_pi4_chip(void)
// {
//     return gpiod_chip_open("/dev/gpiochip0");
// }
// /*
//  * Pi 5: the RP1 drives the header. It was gpiochip4 on early kernels and
//  * is gpiochip0 on current ones, where Raspberry Pi OS keeps gpiochip4 as
//  * a link to it, so gpiochip4 works on both.
//  */
// static struct gpiod_chip *open_pi5_chip(void)
// {
//     return gpiod_chip_open("/dev/gpiochip4");
// }
// /* Open the header GPIO chip for this board, or NULL if it's not a Pi 4/5. */
// static struct gpiod_chip *open_header_chip(void)
// {
//     char model[128] = ""; /* e.g. "Raspberry Pi 5 Model B Rev 1.0" */
//     FILE *f;
//     f = fopen("/proc/device-tree/model", "r");
//     if (f != NULL)
//     {
//         if (fgets(model, sizeof(model), f) == NULL)
//         {
//             model[0] = '\0';
//         }
//         fclose(f);
//     }
//     if (strstr(model, "Raspberry Pi 5") != NULL)
//     {
//         return open_pi5_chip();
//     if (strstr(model, "Raspberry Pi 5") != NULL)
//     {
//         return open_pi5_chip();
//     }
//     if (strstr(model, "Raspberry Pi 4") != NULL)
//     {
//         return open_pi4_chip();
//     }
//     fprintf(stderr, "This program needs a Raspberry Pi 4 or Pi 5.\n");
//     errno = ENODEV;
//     return NULL;
// }
static struct gpiod_chip *open_header_chip(const char *override)
{
    if (override != NULL)
    {
        return gpiod_chip_open(override);
    }

    for (int i = 0; i < 256; i++)
    {
        char path[64];
        int n = snprintf(path, sizeof(path), "/dev/gpiochip%d", i);
        if (n < 0 || (size_t)n >= sizeof(path))
        {
            errno = EINVAL;
            return NULL;
        }
        struct gpiod_chip *chip = gpiod_chip_open(path);
        if (chip == NULL)
        {
            if (errno == ENOENT || errno == ENODEV)
            {
                continue;
}
            perror(path);
            continue;
        }
        struct gpiod_chip_info *info = gpiod_chip_get_info(chip);
        if (info == NULL)
        {
            perror("get chip info");
            gpiod_chip_close(chip);
            continue;
        }
        const char *label = gpiod_chip_info_get_label(info);
        int match = label != NULL &&
            (strcmp(label, "pinctrl-rp1") == 0 ||
             strcmp(label, "pinctrl-bcm2711") == 0);
        gpiod_chip_info_free(info);
        if (match)
        {
            printf("Detected GPIO chip: %s\n", path);
            return chip;
        }
        gpiod_chip_close(chip);
    }
    errno = ENODEV;
    return NULL;
}

// Original Code (disabled):
// struct rgb_led
// {
//     struct gpiod_chip *chip;
//     struct gpiod_line_bulk pins;
//     int common_anode; /* 1 = outputs inverted
// };
//
// static void to_levels(const struct rgb_led *led, int mask, int *levels)
// {
//     int i;
//
//     for (i = 0; i < NUM_PINS; i++)
//     {
//         levels[i] = ((mask >> i) & 1) ^ led->common_anode;
//     }
// }
//
// static int rgb_open(struct rgb_led *led)
// {
//     int levels[NUM_PINS];
//     int ret;
//
//     led->chip = open_header_chip();
//     if (led->chip == NULL)
//     {
//         perror("open GPIO chip");
//         return -1;
//     }
//
//     ret = gpiod_chip_get_lines(led->chip, LED_PINS, NUM_PINS, &led->pins);
//     if (ret != 0)
//     {
//         perror("get GPIO pins");
//         gpiod_chip_close(led->chip);
//         return -1;
//     }
//
//     to_levels(led, 0, levels);
//     if (gpiod_line_request_bulk_output(&led->pins, CONSUMER, levels) != 0)
//     {
//         perror("request GPIO pins");
//         gpiod_chip_close(led->chip);
//         return -1;
//     }
//
//     return 0;
// }
//
// static int rgb_show(struct rgb_led *led, int mask)
// {
//     int levels[NUM_PINS];
//
//     to_levels(led, mask, levels);
//     if (gpiod_line_set_value_bulk(&led->pins, levels) != 0)
//     {
//         perror("set GPIO pins");
//         return -1;
//     }
//
//     return 0;
// }
//
// Turn the LED off, then release the pins and close the chip.
// static int rgb_close(struct rgb_led *led)
// {
//     int ret;
//
//     ret = rgb_show(led, 0);
//     gpiod_chip_close(led->chip);
//
//     return ret;
// }
//


/* Modified for  LIBGPIOD V2: replace line bulk with a line request. */
struct rgb_led
{
    struct gpiod_chip *chip;
    struct gpiod_line_request *request;
    int common_anode;
};

static void to_levels(const struct rgb_led *led, int mask,
                      enum gpiod_line_value *levels)
{
    for (int i = 0; i < NUM_PINS; i++)
    {
        levels[i] = (((mask >> i) & 1) ^ led->common_anode)
            ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE;
    }
}

static int rgb_open(struct rgb_led *led, const char *chip_path)
{
    struct gpiod_line_settings *settings = NULL;
    struct gpiod_line_config *config = NULL;
    struct gpiod_request_config *request_config = NULL;
    enum gpiod_line_value levels[NUM_PINS];
    int ret = -1;

    led->chip = open_header_chip(chip_path);
    if (led->chip == NULL)
    {
        perror("open GPIO chip");
        return -1;
    }
    settings = gpiod_line_settings_new();
config = gpiod_line_config_new();
    request_config = gpiod_request_config_new();
    if (settings == NULL || config == NULL || request_config == NULL)
    {
        perror("allocate GPIO configuration");
        goto cleanup;
    }
    to_levels(led, 0, levels);
    if (gpiod_line_settings_set_direction(settings,
            GPIOD_LINE_DIRECTION_OUTPUT) != 0 ||
        gpiod_line_settings_set_output_value(settings, levels[0]) != 0 ||
        gpiod_line_config_add_line_settings(config, LED_PINS,
            NUM_PINS, settings) != 0)
    {
        perror("configure GPIO pins");
        goto cleanup;
    }
    gpiod_request_config_set_consumer(request_config, CONSUMER);
    led->request = gpiod_chip_request_lines(led->chip, request_config, config);
    if (led->request == NULL)
    {
        perror("request GPIO pins");
        goto cleanup;
    }
    ret = 0;

cleanup:
    if (request_config != NULL) gpiod_request_config_free(request_config);
    if (config != NULL) gpiod_line_config_free(config);
    if (settings != NULL) gpiod_line_settings_free(settings);
    if (ret != 0)
    {
        gpiod_chip_close(led->chip);
        led->chip = NULL;
    }
    return ret;
}
static int rgb_show(struct rgb_led *led, int mask)
{
    enum gpiod_line_value levels[NUM_PINS];
    to_levels(led, mask, levels);
    if (gpiod_line_request_set_values(led->request, levels) != 0)
    {
        perror("set GPIO pins");
        return -1;
    }
    return 0;
}

static int rgb_close(struct rgb_led *led)
{
    int ret = rgb_show(led, 0);
    gpiod_line_request_release(led->request);
    gpiod_chip_close(led->chip);
    return ret;
}


static volatile sig_atomic_t g_stop = 0;

static void on_signal(int sig)
{
    (void)sig;
    g_stop = 1;
}

static int install_handlers(void)
{
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = on_signal;
    // Original Code:  sigemptyset(&sa.sa_mask);
    // Modified: check return value.
    if (sigemptyset(&sa.sa_mask) != 0)
{
        perror("sigemptyset");
        return -1;
    }

    if (sigaction(SIGINT, &sa, NULL) != 0)
    {
        perror("sigaction(SIGINT)");
        return -1;
    }
    if (sigaction(SIGTERM, &sa, NULL) != 0)
    {
        perror("sigaction(SIGTERM)");
        return -1;
    }

    return 0;
}

static int sleep_ms(long ms)
{
    struct timespec req;

    req.tv_sec = ms / 1000;
    req.tv_nsec = (ms % 1000) * 1000000L;

    while (nanosleep(&req, &req) != 0)
    {
        if (errno != EINTR)
        {
            perror("nanosleep");
            return -1;
        }
        if (g_stop)
        {
            break;
        }
    }
    return 0;
}

static const struct color *find_color(const char *name)
{
    int i;

    for (i = 0; i < NUM_COLORS; i++)
    {
        if (strcasecmp(COLORS[i].name, name) == 0)
        {
            return &COLORS[i];
        }
    }

    return NULL;
}


static int parse_long(const char *s, long min, long max, long *out)
{
    char *end;
    long v;

    errno = 0;
    v = strtol(s, &end, 10);

    if (errno != 0 || end == s || *end != '\0')
    {
        return -1;
    }
    if (v < min || v > max)
{
        return -1;
    }

    *out = v;
    return 0;
}

static void usage(const char *prog)
{
    int i;

    fprintf(stderr,
            // Original Code: "Usage: %s [-c color] [-d delay_ms] [-n cycles] [-a] [-h]\n"
            "Usage: %s [-c color] [-d delay_ms] [-n cycles] [-a] [-p chip_path] [-h]\n"
            "  -p path      override GPIO chip path\n"
            "  -c color     show one color until Ctrl+C\n"
            "  -d delay_ms  time per color when cycling (1-%ld, default %ld)\n"
            "  -n cycles    full cycles to run, 0 = until Ctrl+C (default 0)\n"
            "  -a           common-anode LED (invert outputs)\n"
            "  -h           show this help\n"
            "Colors:",
            prog, MAX_DELAY_MS, DEFAULT_DELAY_MS);

    for (i = 0; i < NUM_COLORS; i++)
    {
        fprintf(stderr, " %s", COLORS[i].name);
    }
    fputc('\n', stderr);
}

static int run_single(struct rgb_led *led, const struct color *c)
{
    printf("Showing %s. Press Ctrl+C to stop.\n", c->name);
fflush(stdout);

    if (rgb_show(led, c->mask) != 0)
    {
        return -1;
    }

    while (!g_stop)
    {
        if (sleep_ms(100) != 0)
        {
            return -1;
        }
    }

    return 0;
}

static int run_cycle(struct rgb_led *led, long delay_ms, long cycles)
{
    long done;
    int i;

    printf("Cycling colors every %ld ms. Press Ctrl+C to stop.\n", delay_ms);

    for (done = 0; cycles == 0 || done < cycles; done++)
    {
        for (i = 1; i < NUM_COLORS; i++)
        {
            if (g_stop)
            {
                return 0;
            }

            printf("  %s\n", COLORS[i].name);
fflush(stdout);

            if (rgb_show(led, COLORS[i].mask) != 0)
            {
                return -1;
            }
            if (sleep_ms(delay_ms) != 0)
            {
                return -1;
            }
        }
    }

    return 0;
}

/* ---- Main ---------------------------------------------------------------- */

int main(int argc, char *argv[])
{
    struct rgb_led led = {0};
    const struct color *single = NULL;
    long delay_ms = DEFAULT_DELAY_MS;
    long cycles = 0;
    int status = EXIT_SUCCESS;
    int opt;
    const char *chip_path = NULL;

    // Original Code: while ((opt = getopt(argc, argv, "c:d:n:ah")) != -1)
    // Modified Code: -p chip override.
    while ((opt = getopt(argc, argv, "c:d:n:ap:h")) != -1)
    {
        switch (opt)
        {
            case 'c':
                single = find_color(optarg);
                if (single == NULL)
{
                    fprintf(stderr, "Unknown color: %s\n", optarg);
                    usage(argv[0]);
                    return EXIT_FAILURE;
                }
                break;
            case 'd':
                if (parse_long(optarg, 1, MAX_DELAY_MS, &delay_ms) != 0)
                {
                    fprintf(stderr, "Invalid delay: %s\n", optarg);
                    return EXIT_FAILURE;
                }
                break;
            case 'n':
                if (parse_long(optarg, 0, MAX_CYCLES, &cycles) != 0)
                {
                    fprintf(stderr, "Invalid cycle count: %s\n", optarg);
                    return EXIT_FAILURE;
                }
                break;
            case 'a':
                led.common_anode = 1;
                break;
            case 'p':
                chip_path = optarg;
                break;
            case 'h':
                usage(argv[0]);
                return EXIT_SUCCESS;
            default:
                usage(argv[0]);
                return EXIT_FAILURE;
        }
    }

    if (optind != argc)
{
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    if (install_handlers() != 0)
    {
        return EXIT_FAILURE;
    }
    if (rgb_open(&led, chip_path) != 0)
    {
        return EXIT_FAILURE;
    }

    if (single != NULL)
    {
        if (run_single(&led, single) != 0)
        {
            status = EXIT_FAILURE;
        }
    }
    else
    {
        if (run_cycle(&led, delay_ms, cycles) != 0)
        {
            status = EXIT_FAILURE;
        }
    }

    if (rgb_close(&led) != 0)
    {
        status = EXIT_FAILURE;
    }

    if (g_stop)
{
        printf("\nStopped. LED off, GPIO released.\n");
    }

    return status;
}
