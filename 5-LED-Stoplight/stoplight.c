/*
 Author: Fiza Shaikh
 EET 3150: 3 LED Stop Light
 
 Wiring:
 RED      GPIO 17  (pin 11)
 YELLOW   GPIO 27  (pin 13)
 GREEN    GPIO 22  (pin 15)
 Y_ARROW  GPIO 23  (pin 16)
 G_ARROW  GPIO 24  (pin 18)
 GND               (pin 14)
 */

#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define NUM_LEDS  5

#define RED       (1u << 0)
#define YELLOW    (1u << 1)
#define GREEN     (1u << 2)
#define Y_ARROW   (1u << 3)
#define G_ARROW   (1u << 4)
#define ARROWS    (Y_ARROW | G_ARROW)

#define GPFSEL0   0
#define GPSET0    7
#define GPCLR0    10

const int led_pins[NUM_LEDS] = { 17, 27, 22, 23, 24 };

volatile uint32_t *gpio = NULL;

void gpio_setup(void);
void show(const char *step, unsigned int signal, int seconds);

int main(void)
{
    unsigned int signal;
    int cycle;
    int i;

    gpio_setup();

    signal = RED;
    for (cycle = 0; cycle < 2; cycle++) {
        show("Red", signal, 2);

        signal = signal | G_ARROW;
        show("signal | G_ARROW", signal, 3);

        signal = (signal & ~ARROWS) | Y_ARROW;
        show("(signal & ~ARROWS) | Y_ARROW", signal, 2);

        signal = signal & ~Y_ARROW;
        show("signal & ~Y_ARROW", signal, 1);

        /* Read before write using if (signal & GREEN) */
        if (signal & GREEN) {
            printf("GO\n");
        } else {
            printf("STOP\n");
        }

        signal = signal ^ (RED | GREEN);
        show("signal ^ (RED | GREEN)", signal, 3);

        signal = signal ^ (GREEN | YELLOW);
        show("signal ^ (GREEN | YELLOW)", signal, 2);

        signal = signal ^ (YELLOW | RED);
        show("signal ^ (YELLOW | RED)", signal, 1);
    }

    for (i = 0; i < 6; i++) {
        signal = signal ^ RED;
        show("Flashing: signal ^ RED", signal, 1);
    }

    /* Flash yellow arrow 5 times while red stays on using XOR */
    signal = RED;
    for (i = 0; i < 5; i++) {
        signal = signal ^ Y_ARROW;
        show("Flashing yellow arrow", signal, 1);
    }

    show("All off", 0, 0);
    return 0;
}

void gpio_setup(void)
{
    int fd;
    int i;
    int reg;
    int shift;

    fd = open("/dev/gpiomem", O_RDWR | O_SYNC);
    if (fd < 0) {
        printf("No Raspberry Pi GPIO found - showing LEDs on screen only.\n\n");
        return;
    }

    gpio = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    if (gpio == MAP_FAILED) {
        gpio = NULL;
        printf("Could not map GPIO - showing LEDs on screen only.\n\n");
        return;
    }

    for (i = 0; i < NUM_LEDS; i++) {
        reg   = led_pins[i] / 10;
        shift = (led_pins[i] % 10) * 3;

        gpio[GPFSEL0 + reg] &= ~(7u << shift);
        gpio[GPFSEL0 + reg] |=  (1u << shift);
    }
}

void show(const char *step, unsigned int signal, int seconds)
{
    int i;
    unsigned int bit;

    printf("%-30s ", step);
    for (i = NUM_LEDS - 1; i >= 0; i--) {
        bit = (signal >> i) & 1u;
        printf("%u", bit);
    }
    printf("\n");

    if (gpio != NULL) {
        for (i = 0; i < NUM_LEDS; i++) {
            bit = (signal >> i) & 1u;
            if (bit == 1) {
                gpio[GPSET0] = 1u << led_pins[i];
            } else {
                gpio[GPCLR0] = 1u << led_pins[i];
            }
        }
    }

    sleep(seconds);
}
