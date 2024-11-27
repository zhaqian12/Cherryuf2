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

#ifndef BOARDS_H_
#define BOARDS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"
#include "stm32f4xx_hal_conf.h"

#include "board.h"

// Flash Start Address of Application
#ifndef CONFIG_BOOTUF2_APP_START_ADDR
#    define CONFIG_BOOTUF2_APP_START_ADDR 0x08004000
#endif

#ifndef CONFIG_BOOTUF2_APP_END_ADDR
#    define CONFIG_BOOTUF2_APP_END_ADDR (0x08000000 + BOARD_FLASH_SIZE)
#endif

#ifndef CONFIG_BOOTUF2_PAGE_COUNTMAX
#    define CONFIG_BOOTUF2_PAGE_COUNTMAX 128
#endif

#ifndef CONFIG_BOOTUF2_CACHE_SIZE
#    define CONFIG_BOOTUF2_CACHE_SIZE 4096
#endif

// Double Reset tap to enter DFU
#define CHERRYUF2_DFU_DOUBLE_TAP 1

#ifdef __cplusplus
}
#endif

#endif /* BOARDS_H_ */
