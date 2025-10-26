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


#ifndef INC_HID_DEVICE_H_
#define INC_HID_DEVICE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "tusb.h"
#include "stm32f4xx.h"
#include "packet_queue.h"

#define HID_KEYBOARD 0x01
#define HID_MOUSE 0x02

#define REPORT_ID_KEYBOARD  0x01
#define REPORT_ID_MOUSE     0x02

extern volatile bool hid_device;
extern queue_t incoming_queue;

void hid_task();

#endif /* INC_HID_DEVICE_H_ */
