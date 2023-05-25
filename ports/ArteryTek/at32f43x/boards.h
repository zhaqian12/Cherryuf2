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

#ifndef BOARDS_H_
#define BOARDS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "at32f435_437.h"

#include "board.h"

// Flash Start Address of Application
#ifndef BOARD_FLASH_APP_START
#    define BOARD_FLASH_APP_START 0x08004000
#endif

// Double Reset tap to enter DFU
#define CHERRYUF2_DFU_DOUBLE_TAP 1

// OTGFS Config
#ifdef AT32_USE_OTGFS2
#define OTG_PORT            GPIOB
#define OTG_DM_PIN          GPIO_PINS_14
#define OTG_DM_MUX_SOURCE   GPIO_PINS_SOURCE14
#define OTG_DP_PIN          GPIO_PINS_15
#define OTG_DP_MUX_SOURCE   GPIO_PINS_SOURCE15
#define OTG_SOF_PORT        GPIOA
#define OTG_SOF_PIN         GPIO_PINS_4
#define OTG_SOF_MUX_SOURCE  GPIO_PINS_SOURCE4
#define OTG_VBUS_PIN        GPIO_PINS_13
#define OTG_VBUS_MUX_SOURCE GPIO_PINS_SOURCE13
#define OTG_PERIPH_CLOCK    CRM_OTGFS2_PERIPH_CLOCK
#define OTG_IRQ             OTGFS2_IRQn
#else
#define OTG_PORT            GPIOA
#define OTG_DM_PIN          GPIO_PINS_11
#define OTG_DM_MUX_SOURCE   GPIO_PINS_SOURCE11
#define OTG_DP_PIN          GPIO_PINS_12
#define OTG_DP_MUX_SOURCE   GPIO_PINS_SOURCE12
#define OTG_SOF_PORT        GPIOA
#define OTG_SOF_PIN         GPIO_PINS_8
#define OTG_SOF_MUX_SOURCE  GPIO_PINS_SOURCE8
#define OTG_VBUS_PIN        GPIO_PINS_9
#define OTG_VBUS_MUX_SOURCE GPIO_PINS_SOURCE9
#define OTG_PERIPH_CLOCK    CRM_OTGFS1_PERIPH_CLOCK
#define OTG_IRQ             OTGFS1_IRQn
#endif

#ifdef __cplusplus
}
#endif

#endif /* BOARDS_H_ */
