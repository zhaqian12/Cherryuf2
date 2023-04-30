/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Ha Thach for Adafruit Industries
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

#ifndef BOARDS_H
#define BOARDS_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "boards.h"

//--------------------------------------------------------------------+
// Features
//--------------------------------------------------------------------+

// Flash Start Address of Application
#ifndef BOARD_FLASH_APP_START
#define BOARD_FLASH_APP_START  0
#endif

// Use Double Tap method to enter DFU mode
#ifndef CHERRYUF2_DFU_DOUBLE_TAP
#define CHERRYUF2_DFU_DOUBLE_TAP      1
#endif

// Write protection for bootloader
#ifndef CHERRYUF2_PROTECT_BOOTLOADER
#define CHERRYUF2_PROTECT_BOOTLOADER  0
#endif

#ifndef CHERRYUF2_MSC_WRITE_COMPLETE_TIMEOUT
#define CHERRYUF2_MSC_WRITE_COMPLETE_TIMEOUT  100
#endif

//--------------------------------------------------------------------+
// Constant
//--------------------------------------------------------------------+

#define DBL_TAP_MAGIC            0xf01669ef // Enter DFU magic
#define DBL_TAP_MAGIC_QUICK_BOOT 0xf02669ef // Skip double tap delay detection
#define DBL_TAP_MAGIC_ERASE_APP  0xf5e80ab4 // Erase entire application !!

//--------------------------------------------------------------------+
// Basic API
//--------------------------------------------------------------------+

// Baudrate for UART if used
#define BOARD_UART_BAUDRATE   115200

void board_init(void);

void board_led_write(uint32_t value);

int board_uart_write(void const * buf, int len);

void board_timer_start(uint32_t ms);

void board_timer_stop(void);

extern void board_timer_handler(void);

bool board_app_valid(void);

void board_app_jump(void);

void board_dfu_complete(void);

//--------------------------------------------------------------------+
// Flash API
//--------------------------------------------------------------------+

void board_flash_init(void);

uint32_t board_flash_size(void);

void board_flash_read (uint32_t addr, void* buffer, uint32_t len);

void board_flash_write(uint32_t addr, void const *data, uint32_t len);

void board_flash_flush(void);

void board_flash_erase_app(void);

bool board_flash_protect_bootloader(bool protect);

void board_self_update(const uint8_t * bootloader_bin, uint32_t bootloader_len);

#endif
