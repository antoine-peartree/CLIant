/*
 * leds.h
 *
 *  Created on: Oct 24, 2020
 *      Author: antoinepoirier
 */

#ifndef INC_LEDS_H_
#define INC_LEDS_H_

#include "main.h"

struct led_connection {
	char* label;
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;
};

int leds_stoi(char *led_state);

int leds_get_connection(char *label, GPIO_TypeDef **GPIOx, uint16_t *GPIO_Pin);

#endif /* INC_LEDS_H_ */
