/*
 * manchester.h
 *
 *  Created on: Aug 23, 2025
 *      Author: benlilburne
 */

#ifndef INC_MANCHESTER_H_
#define INC_MANCHESTER_H_

#include "stm32f4xx_hal.h"
#include <string.h>

#define OUTPUT_PORT GPIOA
#define OUTPUT_PIN 5

void manchester_encode(uint8_t* data, size_t nybtes, size_t* out_len);
HAL_StatusTypeDef manchester_transmit(const uint8_t* data, size_t nbytes);

extern TIM_HandleTypeDef htim2;
extern DMA_HandleTypeDef hdma_tim2_up_ch3;

#endif /* INC_MANCHESTER_H_ */
