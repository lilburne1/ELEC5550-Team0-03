/*
 * manchester.c
 *
 *  Created on: Aug 23, 2025
 *      Author: benlilburne
 */
#include "manchester.h"
#include "stm32f4xx_hal.h"
#define MANCHESTER_HALFBITS 1024
static uint32_t manchester_buffer[MANCHESTER_HALFBITS];


// Handles Encoding of Packet
void manchester_encode(uint8_t* data, size_t nbytes, size_t* out_length) {
// Used to set the output pin either high or low
	uint32_t high = (1U << OUTPUT_PIN);
	uint32_t low = (1U << (OUTPUT_PIN + 16));

	uint8_t clock_phase = 1;
	uint8_t write_index = 0;

// Iterate over all bits in packet
	for(size_t i=0; i < nbytes; ++i) {
		uint8_t byte = data[i];
		for(size_t j=0; j < 8; ++j) {
			uint8_t bit = (byte >> j) & 1U;

			// First half bit - Manchester encoding through XOR
			uint8_t output_bit1 = bit ^ clock_phase;
			if(output_bit1) {
				manchester_buffer[write_index] = high;
			}
			else {
				manchester_buffer[write_index] = low;
			}
			write_index++;
			clock_phase ^= 1;

			// Second half bit
			uint8_t output_bit2 = bit ^ clock_phase;
			if(output_bit2) {
				manchester_buffer[write_index] = high;
			}
			else {
				manchester_buffer[write_index] = low;
			}
			write_index++;
			clock_phase ^= 1;
		}
	}
	*out_length = write_index;
}

HAL_StatusTypeDef manchester_transmit(const uint8_t* data, size_t nbytes) {
	GPIOA->BSRR = (1U << (5 + 16U));

	// Encodes data using established function
	size_t len = 0;
	manchester_encode(data, nbytes, &len);

	HAL_StatusTypeDef status = HAL_DMA_Start_IT(&hdma_tim2_up_ch3, (uint32_t) manchester_buffer, (uint32_t)&(GPIOA->BSRR), (uint32_t) len);
	__HAL_TIM_ENABLE_DMA(&htim2, TIM_DMA_UPDATE);
	__HAL_TIM_DISABLE(&htim2);
	return HAL_OK;
}

void HAL_DMA_XferCpltCallback(DMA_HandleTypeDef *hdma) {
	if(hdma == &hdma_tim2_up_ch3) {
		__HAL_TIM_DISABLE_DMA(&htim2, TIM_DMA_UPDATE);
		__HAL_TIM_DISABLE(&htim2);
		GPIOA->BSRR = (1U << (OUTPUT_PIN + 16));
	}
}


