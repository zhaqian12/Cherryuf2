/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2024 Zhaqian
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
 */

#ifndef BOOTUF2_DESC_H
#define BOOTUF2_DESC_H

#include "usbd_core.h"
#include "usbd_msc.h"

#define BOOTUF2_IN_EP 0x81
#define BOOTUF2_OUT_EP 0x02

#define BOOTUF2_MAX_MPS_HS 512
#define BOOTUF2_MAX_MPS_FS 64

#define USBD_MAX_POWER 100
#define USBD_LANGID_STRING 1033

#ifndef USB_MANUFACTURER
#    define USB_MANUFACTURER "CherryUF2"
#endif

#ifndef USB_PRODUCT
#    define USB_PRODUCT "CherryUSB UF2 Bootloader"
#endif

#ifndef BOOTUF2_BUS_ID_FS
#    define BOOTUF2_BUS_ID_FS 0x00
#endif

#ifndef BOOTUF2_BUS_ID_HS
#    define BOOTUF2_BUS_ID_HS 0x01
#endif

extern const struct usb_descriptor bootuf2_descriptor;

void usbd_event_handler(uint8_t busid, uint8_t event);

#endif
