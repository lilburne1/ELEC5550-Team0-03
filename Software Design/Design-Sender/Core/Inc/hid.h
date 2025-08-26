/*
 * hid.h
 *
 *  Created on: Aug 24, 2025
 *      Author: benlilburne
 */

#ifndef INC_HID_H_
#define INC_HID_H_

#include <stdint.h>
#include <string.h>
#include "bsp/board_api.h"
#include "tusb.h"
#include "stm32f4xx_hal.h"

#define QUEUE_LENGTH 16
#define REPORT_LENGTH 64

typedef struct {
	uint8_t len;
	uint8_t data[REPORT_LENGTH];
} hid_packet;

typedef struct {
	volatile uint16_t head;
	volatile uint16_t tail;
	hid_packet buffer[QUEUE_LENGTH];
} hid_queue;

typedef struct {
	uint8_t bufferA[REPORT_LENGTH];
	uint8_t bufferALength;

	uint8_t bufferB[REPORT_LENGTH];
	uint8_t bufferBLength;

	bool currentBuffer;
	uint16_t currentIndex;
	bool transmitBusy;
} sending_buffer;


void hid_queue_init(hid_queue *q);

void hid_packet_create(hid_packet *p, const uint8_t *report, uint16_t len);

bool hid_queue_push(hid_queue *q, const hid_packet *p);

bool hid_queue_pop(hid_queue *q, hid_packet *out);

void transmit_hid();

#endif /* INC_HID_H_ */
