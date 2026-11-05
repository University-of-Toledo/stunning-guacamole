/*
 * Author: Zachary Wertz
 * Exercise: Lab 13 - Traffic Light
 * Description: Data-driven traffic light controller using libgpiod.
 */

#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define CHIP_PATH "/dev/gpiochip0"	// "/dev/gpiochip0"  is needed due to the version of gpiod that is installed//


// Defines the offests array to match the pin defionition //

#define Pin_Red    17
#define Pin_Yellow 27
#define Pin_Green  22

typedef struct {
	int red_state;
	int yellow_state;
	int green_state;
	int duration_sec;
} traffic_state_t;

int main(void) {
	struct gpiod_chip *chip;
	struct gpiod_line_settings *settings;
	struct gpiod_line_config *line_cfg;
	struct gpiod_request_config *req_cfg;
	struct gpiod_line_request *request;

    // Define the state table sequence (Red -> Green -> Yellow -> Red)
    traffic_state_t states[] = {
        { 1, 0, 0, 3 }, // Red on for 3 seconds
        { 0, 0, 1, 3 }, // Green on for 3 seconds
        { 0, 1, 0, 2 }, // Yellow on for 1 seconds
	{ 1, 0, 0, 3 }  // Ends with Red on for 3 seconds
};

int num_states = sizeof(states) / sizeof(states[0]);

// Open the gpio chip
chip = gpiod_chip_open(CHIP_PATH);
if(!chip) {
	perror("Failed to Open the Chip");
	return 1;
}

// Configure line settings as Outputs
settings = gpiod_line_settings_new();
gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);

// Defines the offsets and add settings to the configuration
line_cfg = gpiod_line_config_new();
unsigned int offsets[] = {Pin_Red, Pin_Yellow, Pin_Green};
gpiod_line_config_add_line_settings(line_cfg, offsets, 3, settings); //test add
req_cfg = gpiod_request_config_new(); //test add
gpiod_request_config_set_consumer(req_cfg, "Traffic_app");

// Request exclusive line control of the chip
request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);
if (!request) {
	perror("Failed to request lines");
	gpiod_line_config_free(line_cfg);
	gpiod_line_settings_free(settings);
	gpiod_request_config_free(req_cfg);
	gpiod_chip_close(chip);
	return 1;
}

printf("Starting the Traffic Light Sequence.\n");

// This is the loop of the traffic light sequence
for (int i = 0; i < num_states; i ++) {
	// Map states to libgpiod enum values (Active = On, Inactive = Off)
	enum gpiod_line_value values[3];
	values[0] = states[i].red_state    ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE;
	values[1] = states[i].yellow_state ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE;
	values[2] = states[i].green_state  ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE;

// Applys the values to the pins matching the offset array order [ Red, Yellow, Green]
gpiod_line_request_set_values(request, values);

printf("State %d -> Red: %d, Yellow: %d, Green: %d (Hold for %ds)\n",
	i + 1, states[i].red_state, states[i].yellow_state, states[i].green_state, states[i].duration_sec);

	sleep(states[i].duration_sec);
}

printf(" Traffic Sequence completed sucessfully.\n");

// Cleans up the resources and turns off all lights safely
enum gpiod_line_value turn_off[] = {GPIOD_LINE_VALUE_INACTIVE, GPIOD_LINE_VALUE_INACTIVE, GPIOD_LINE_VALUE_INACTIVE};
gpiod_line_request_set_values(request, turn_off);

gpiod_line_request_release(request);
gpiod_line_config_free(line_cfg);
gpiod_line_settings_free(settings);
gpiod_request_config_free(req_cfg);
gpiod_chip_close(chip);

return 0;
}
