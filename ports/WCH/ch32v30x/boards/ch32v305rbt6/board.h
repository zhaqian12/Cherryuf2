/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 Zhaqian
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

#ifndef BOARD_H_
#define BOARD_H_

#include "ch32v30x.h"

// clang-format off
//--------------------------------------------------------------------+
// LED
//--------------------------------------------------------------------+
#define LED_PORT             GPIOA
#define LED_PIN              GPIO_Pin_3
#define LED_STATE_ON         0

//--------------------------------------------------------------------+
// FLASH
//--------------------------------------------------------------------+
#define BOARD_SECTOR_SIZE   4096U
#define BOARD_SECTOR_COUNT  36
#define BOARD_FLASH_SIZE    (BOARD_SECTOR_SIZE * BOARD_SECTOR_COUNT)

//--------------------------------------------------------------------+
// USB UF2
//--------------------------------------------------------------------+
#define USBD_VID            0x00AA
#define USBD_PID            0xAAFF
#define USB_MANUFACTURER    "WCH"
#define USB_PRODUCT         "CH32V305"

#define UF2_PRODUCT_NAME    USB_MANUFACTURER " " USB_PRODUCT
#define UF2_BOARD_ID        "CH32V305xB"
#define UF2_VOLUME_LABEL    "CherryUF2"
#define UF2_INDEX_URL       "https://www.wch.cn/products/CH32V305.html"

// clang-format on
//--------------------------------------------------------------------+
// CLOCK
//--------------------------------------------------------------------+
static inline void clock_init(void) {
    // TODO
}

#endif
