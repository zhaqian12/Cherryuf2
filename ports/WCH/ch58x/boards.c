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

#include "board_api.h"

//--------------------------------------------------------------------+
// CherryUSB LLD
//--------------------------------------------------------------------+
__attribute__((weak)) void usb_dc_low_level_init(void) {
    extern void USB_IRQHandler(void);
    PFIC_EnableIRQ(USB_IRQn);
    PFIC_EnableFastINT0(USB_IRQn, (uint32_t)(void *)USB_IRQHandler);
}

//--------------------------------------------------------------------+
// Boards api
//--------------------------------------------------------------------+
__attribute__((weak)) void board_init(void) {
    clock_init();
    board_timer_stop();

#ifdef LED_PIN
    LED_PIN_MODE_CFG(LED_PIN, GPIO_ModeOut_PP_20mA);

    board_led_write(1);
#endif
}

__attribute__((weak)) void board_dfu_complete(void) {
    SYS_ResetExecute();
}

__attribute__((weak)) void board_usb_process(void) {
    // todo
}

__attribute__((weak)) bool board_app_valid(void) {
    // need to improve
    const uint32_t val = *(volatile uint32_t const *)BOARD_FLASH_APP_START;

    return (val != 0xf3f9bda9);
}

__attribute__((weak)) void board_app_jump(void) {
#ifdef LED_PIN
    LED_PIN_MODE_CFG(LED_PIN, GPIO_ModeIN_Floating);
#endif
    R32_USB_CONTROL = 0;

    SysTick->CTLR = 0;
    SysTick->SR   = 0;
    SysTick->CNT  = 0;
    SysTick->CMP  = 0;

    uint32_t irq_status;
    SYS_DisableAllIrq(&irq_status);

    BOOT_JUMP();
}

//--------------------------------------------------------------------+
// LED
//--------------------------------------------------------------------+
void board_led_write(uint32_t state) {
    (void)state;
#ifdef LED_PIN
    uint32_t sts = state ? LED_STATE_ON : (1 - LED_STATE_ON);
    if (sts) {
        LED_PIN_SETBITS(LED_PIN);
    } else {
        LED_PIN_RESETBITS(LED_PIN);
    }
#endif
}

//--------------------------------------------------------------------+
// Timer
//--------------------------------------------------------------------+
void board_timer_start(uint32_t ms) {
    SysTick_Config((GetSysClock() / 1000) * ms);
}

void board_timer_stop(void) {
    SysTick->CTLR &= ~(1 << 0);
}

__INTERRUPT
__HIGH_CODE
void SysTick_Handler(void) {
    SysTick->SR = 0;
    board_timer_handler();
}

int board_uart_write(void const *buf, int len) {
    (void)buf;
    (void)len;
    return 0;
}

void _init(void) {}
