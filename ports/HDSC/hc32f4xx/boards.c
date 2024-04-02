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
// Boards api
//--------------------------------------------------------------------+
__attribute__((weak)) void board_init(void) {
    LL_PERIPH_WE(LL_PERIPH_GPIO | LL_PERIPH_EFM | LL_PERIPH_FCG | LL_PERIPH_PWC_CLK_RMU | LL_PERIPH_SRAM);
    EFM_FWMC_Cmd(ENABLE);
    clock_init();
    SystemCoreClockUpdate();
    board_timer_stop();
}

__attribute__((weak)) void board_dfu_init(void) {
#ifdef LED_PIN
    stc_gpio_init_t stcGpioInit;
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDir = PIN_DIR_OUT;
    (void)GPIO_Init(LED_PORT, LED_PIN, &stcGpioInit);

    board_led_write(1);
#endif
}

__attribute__((weak)) void board_dfu_complete(void) {
    NVIC_SystemReset();
}

__attribute__((weak)) bool board_app_valid(void) {
    volatile uint32_t const *app_vector = (volatile uint32_t const *)BOARD_FLASH_APP_START;

    // 2nd word is App entry point (reset)
    if (app_vector[1] < BOARD_FLASH_APP_START || app_vector[1] > BOARD_FLASH_APP_START + BOARD_FLASH_SIZE) {
        return false;
    }

    return true;
}

__attribute__((weak)) void board_app_jump(void) {
    EFM_FWMC_Cmd(DISABLE);
    clock_deinit();
    LL_PERIPH_WP(LL_PERIPH_EFM | LL_PERIPH_FCG | LL_PERIPH_SRAM);

    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL  = 0;

    // Disable all Interrupts
    for (int i = 0; i < BOARD_ARRAY_SIZE(NVIC->ICER); i++) {
        NVIC->ICER[i] = 0xFFFFFFFF;
        NVIC->ICPR[i] = 0xFFFFFFFF;
    }

    volatile uint32_t const *app_vector = (volatile uint32_t const *)BOARD_FLASH_APP_START;

    /* switch exception handlers to the application */
    SCB->VTOR = (uint32_t)BOARD_FLASH_APP_START;

    __set_CONTROL(0);
    __set_MSP(app_vector[0]);
    asm("bx %0" ::"r"(app_vector[1]));
}

//--------------------------------------------------------------------+
// LED
//--------------------------------------------------------------------+
void board_led_write(uint32_t state) {
    (void)state;
#ifdef LED_PIN
    uint32_t sts = state ? LED_STATE_ON : (1 - LED_STATE_ON);
    if (sts) {
        GPIO_SetPins(LED_PORT, LED_PIN);
    } else {
        GPIO_ResetPins(LED_PORT, LED_PIN);
    }
#endif
}

//--------------------------------------------------------------------+
// Timer
//--------------------------------------------------------------------+
void board_timer_start(uint32_t ms) {
    SysTick_Config((SystemCoreClock / 1000) * ms);
}

void board_timer_stop(void) {
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void) {
    board_timer_handler();
}

void _init(void) {}
