/*
Steven cooper
Assignemnt-Blink
Write code and wiring a raspberry pi  to toggle an LED at a certin GPIO
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <gpiod.h>

#define CHIP_PATH "/dev/gpiochip4"
#define BLINK_COUNT 20
#define DELAY_SEC 1

int main(void) {
	struct gpiod_chip *chip = NULL;
	struct gpiod_line_settings *settings = NULL;
	struct gpiod_line_config *line_cfg = NULL;
	struct gpiod_line_request *request = NULL;

	unsigned int offset = 17;
	int i;
	int ret;
	int value = 0;
	enum gpiod_line_value line_val;
	struct timespec req;

	req.tv_sec = DELAY_SEC;
	req.tv_nsec = 0;

	chip = gpiod_chip_open(CHIP_PATH);
	if (!chip) {
		perror("Error opening GPIO chip");
		return EXIT_FAILURE;
	}

	settings = gpiod_line_settings_new();
	if (!settings) {
		perror("Error creating line settings");
		gpiod_chip_close(chip);
		return EXIT_FAILURE;
	}
	line_cfg = gpiod_line_config_new();
	if (!line_cfg) {
		perror("Error creating line config");
		gpiod_line_settings_free(settings);
		gpiod_chip_close(chip);
		return EXIT_FAILURE;
	}
	ret = gpiod_line_config_add_line_settings(line_cfg, &offset, 1, settings);
	if (ret < 0) {
		perror("Error adding line settings");
		gpiod_line_config_free(line_cfg);
		gpiod_line_settings_free(settings);
		gpiod_chip_close(chip);
		return EXIT_FAILURE;
	}

	request = gpiod_chip_request_lines(chip, NULL, line_cfg);
	if (!request) {
		perror("Error requestings lines");
		gpiod_line_config_free(line_cfg);
		gpiod_line_settings_free(settings);
		gpiod_chip_close(chip);
		return EXIT_FAILURE;
	}
	printf("Starting LED blink for %d cycles... \n", BLINK_COUNT);


	for (i = 0; i < BLINK_COUNT; i++) {
		value = !value;
		line_val = value ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE;

		ret = gpiod_line_request_set_value(request, offset, line_val);
		if (ret < 0) {
			perror("Error setting line value");
			gpiod_line_request_release(request);
			gpiod_chip_close(chip);
			return EXIT_FAILURE;
		}
		printf("LED state: %s\n", value ? "ON" : "OFF");
		if (nanosleep(&req, NULL) < 0) {
			perror("Error in nanosleep");
			}
		}
	gpiod_line_request_release(request);
	gpiod_chip_close(chip);
	printf("Blink program finished successfully.\n");
	return EXIT_SUCCESS;
}

