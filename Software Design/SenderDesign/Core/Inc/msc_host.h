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


#ifndef MSC_HOST_H
#define MSC_HOST_H

#include "laser_rx_sender.h"
#include "laser_tx_sender.h"
#include "main.h"
#include "packet_queue.h"

#define MSC_START 0x21
#define MSC_END 0x22
#define MSC_READ 0x30
#define MSC_WRITE 0x31
#define MSC_READ_RESPONSE 0x32
#define MSC_WRITE_RESPONSE 0x33

#define UART_RX_BUFSIZE   1024

extern bool uart_busy;
extern uint8_t uart_txbuffer[UART_TX_MAX];
extern size_t uart_txlen;
extern uint32_t transmission_time;
extern volatile bool acknowledge_received;
extern volatile bool retransmission_needed;
extern uint8_t received_buffer[UART_RX_BUFSIZE];
extern uint32_t read_index;

void handle_msc_request(packet_t *pkt);

#endif
