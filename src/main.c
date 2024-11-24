/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *               2024 Zhaqian  
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

#include <stdbool.h>

#include "bootuf2.h"
#include "usbd_core.h"

// timeout for double tap detection
#define DBL_TAP_DELAY 500

#ifndef DBL_TAP_REG
// defined by linker script
extern uint32_t _board_dfu_dbl_tap[];
#    define DBL_TAP_REG _board_dfu_dbl_tap[0]
#endif

volatile uint32_t _timer_count = 0;

static bool check_dfu_mode(void);
static void bootuf2_task_process(void);

int main(void) {
    board_init();

    // if not DFU mode, jump to App
    if (!check_dfu_mode()) {
        board_app_jump();
        while (1) {
        }
    }

    board_dfu_init();
    board_flash_init();
    board_uf2boot_init();

    while (1) {
        bootuf2_task_process();
    }
}

// return true if start DFU mode, else App mode
static bool check_dfu_mode(void) {
#if CHERRYUF2_DFU_DOUBLE_TAP
    if (DBL_TAP_REG == DBL_TAP_MAGIC_ERASE_APP) {
        DBL_TAP_REG = 0;
        board_flash_erase_app();
    }
#endif

    // Check if app is valid
    if (!board_app_valid()) return true;

#if CHERRYUF2_DFU_DOUBLE_TAP
    if (DBL_TAP_REG == DBL_TAP_MAGIC_QUICK_BOOT) {
        DBL_TAP_REG = 0;
        return false;
    }

    if (DBL_TAP_REG == DBL_TAP_MAGIC) {
        // Double tap occurred
        DBL_TAP_REG = 0;
        return true;
    }

    // Register our first reset for double reset detection
    DBL_TAP_REG = DBL_TAP_MAGIC;

    _timer_count = 0;
    board_timer_start(1);

    // delay a fraction of second if Reset pin is tap during this delay --> we will enter dfu
    while (_timer_count < DBL_TAP_DELAY) {
    }
    board_timer_stop();

    DBL_TAP_REG = 0;
#endif

    return false;
}

static void bootuf2_task_process(void) {
    if (bootuf2_is_write_done()) {
        board_dfu_complete();
    }
    board_user_task_process();
    __NOP();
}

void board_timer_handler(void) {
    _timer_count++;
}
