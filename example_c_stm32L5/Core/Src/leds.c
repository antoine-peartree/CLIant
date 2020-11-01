/*
 * leds.c
 *
 *  Created on: Oct 24, 2020
 *      Author: antoinepoirier
 */
#include <stdio.h>
#include <string.h>
#include "leds.h"

static struct led_connection leds_connections[] = {
		{"GREEN", LED_GREEN_GPIO_Port, LED_GREEN_Pin},
		{"RED", LED_RED_GPIO_Port, LED_RED_Pin}
};

int leds_stoi(char *led_state) {
	if (!strcmp("ON", led_state)) {
		return 1;
	} else if (!strcmp("OFF", led_state)) {
		return 0;
	} else {
		return -1;
	}
}

int leds_get_connection(char *label, GPIO_TypeDef **GPIOx, uint16_t *GPIO_Pin)
{
	for(int i = 0; i < sizeof(leds_connections)/sizeof(leds_connections[0]); i++) {
		if (!strcmp(leds_connections[i].label, label)) {
			*GPIOx = leds_connections[i].GPIOx;
			*GPIO_Pin = leds_connections[i].GPIO_Pin;
			return 0;
		}
	}
	printf("LED connection not found : %s\r\n", label);
	return -1;
}
