/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
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

#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// MCU / OS
#define CFG_TUSB_MCU          OPT_MCU_STM32F4
#define CFG_TUSB_OS           OPT_OS_NONE
#define CFG_TUSB_DEBUG        2

// -------- RHPort mode: HOST on FS --------
//#define CFG_TUSB_RHPORT0_MODE   (OPT_MODE_HOST | OPT_MODE_FULL_SPEED)
#define CFG_TUSB_RHPORT1_MODE   (OPT_MODE_HOST | OPT_MODE_FULL_SPEED)

#define CFG_TUH_ENUMERATION_BUFSIZE 512

// -------- Enable stacks --------
#define CFG_TUH_ENABLED         1       // Host stack ON
#define CFG_TUD_ENABLED         0       // Device stack OFF (explicit)

// -------- Host class config --------
#define CFG_TUH_HID             1
#define CFG_TUH_MSC             1
#define CFG_TUH_MSC_MAXLUN      4
#define CFG_TUH_HUB             0
#define CFG_TUH_HID_EPIN_BUFSIZE   64
#define CFG_TUH_HID_EPOUT_BUFSIZE  64

#ifndef CFG_TUSB_MEM_SECTION
#define CFG_TUSB_MEM_SECTION
#endif
#ifndef CFG_TUSB_MEM_ALIGN
#define CFG_TUSB_MEM_ALIGN      __attribute__ ((aligned(4)))
#endif

#ifdef __cplusplus
}
#endif
#endif // _TUSB_CONFIG_H_


