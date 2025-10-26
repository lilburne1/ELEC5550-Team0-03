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


#ifndef INC_LASER_TX_RECEIVER_H_
#define INC_LASER_TX_RECEIVER_H_

#include <hid_device.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "stm32f4xx_hal.h"
#include "main.h"

#define UART_TX_MAX 1024

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

#define HID_RETRANSMISSION_TIME 200
#define HID_RETRANSMISSION_TIMEOUT 400
#define MSC_RETRANSMISSION_TIME 500u

extern UART_HandleTypeDef huart2;

void uart_tx_service_receiver(void);
void queue_acknowledgement();
void queue_not_acknowledgement();
uint16_t crc16(const uint8_t *data, size_t len);
size_t slip_encode(uint8_t *data, size_t len, uint8_t *out, size_t out_len);

#endif /* INC_LASER_TX_RECEIVER_H_ */
