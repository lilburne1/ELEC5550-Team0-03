/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2025 ELEC5550 Team0-03
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef INC_LASER_RX_SENDER_H_
#define INC_LASER_RX_SENDER_H_

#include "hid_host.h"
#include "laser_tx_sender.h"
#include "stm32f4xx_hal.h"
#include "msc_host.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef  hdma_uart2_rx;

#define UART_RX_BUFSIZE   1024

#define CONTROL_ACK 0x41
#define CONTROL_NACK 0x42
#define HID_KEYBOARD 0x01
#define HID_MOUSE 0x02
#define MSC_START 0x21
#define MSC_END 0x22
#define MSC_READ 0x30
#define MSC_WRITE 0x31
#define MSC_READ_RESPONSE 0x32
#define MSC_WRITE_RESPONSE 0x33

#define SLIP_END 0xC0
#define SLIP_ESC 0xDB
#define SLIP_ESC_END 0xDC
#define SLIP_ESC_ESC 0xDD

typedef struct {
	uint8_t *data;
	size_t length;
	size_t capacity;
	bool escape;
	bool dropping;
} data_stream_t;

extern data_stream_t data_stream;

void uart_rx_service_sender(void);
void uart_rx_init(void);

extern queue_t q;

bool slip_decode(uint8_t b, size_t *frame_length, bool *frame_dropped);


#endif /* INC_LASER_RX_SENDER_H_ */
