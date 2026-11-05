/*
* Author: Zachary Wertz
* Exercise: Blink Lab 
* Description: This program completes the requirements of the test 1 document.
*/

#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

#define CHIP_NAME "/dev/gpiochip4"
#define LINE_OFFSET 17

int main(void) {
    struct gpiod_chip *chip;
    struct gpiod_line_settings *settings;
    struct gpiod_line_config *line_cfg;
    struct gpiod_request_config *req_cfg;
    struct gpiod_line_request *request;
    unsigned int offsets[] = { LINE_OFFSET };
    int ret;

    // 1. Open the GPIO chip
    chip = gpiod_chip_open(CHIP_NAME);
    if (!chip) {
        perror("Failed to open chip");
        return 1;
    }

    // 2. Create line settings and set direction to output
    settings = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);

    // 3. Configure the line
    line_cfg = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(line_cfg, offsets, 1, settings);

    // 4. Configure the request
    req_cfg = gpiod_request_config_new();
    gpiod_request_config_set_consumer(req_cfg, "blink_app");

    // 5. Request the line
    request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);
    
    // Free configuration objects as they are no longer needed after requesting
    gpiod_line_settings_free(settings);
    gpiod_line_config_free(line_cfg);
    gpiod_request_config_free(req_cfg);

    if (!request) {
        perror("Failed to request line");
        gpiod_chip_close(chip);
        return 1;
    }

    // 6. Blink loop
    for (int i = 0; i < 10; i++) {
        // Set High (1)
        gpiod_line_request_set_value(request, LINE_OFFSET, GPIOD_LINE_VALUE_ACTIVE);
        sleep(1);

        // Set Low (0)
        gpiod_line_request_set_value(request, LINE_OFFSET, GPIOD_LINE_VALUE_INACTIVE);
        sleep(1);
    }

    // 7. Clean up
    gpiod_line_request_release(request);
    gpiod_chip_close(chip);

    return 0;
}
