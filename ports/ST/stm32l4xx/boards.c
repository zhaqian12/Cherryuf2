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

#include "board_api.h"

//--------------------------------------------------------------------+
// Boards api
//--------------------------------------------------------------------+
__attribute__((weak)) void board_init(void) {
    clock_init();
    SystemCoreClockUpdate();
    board_timer_stop();
}

__attribute__((weak)) void board_dfu_init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
#ifdef GPIOD
    __HAL_RCC_GPIOD_CLK_ENABLE();
#endif
#ifdef GPIOE
    __HAL_RCC_GPIOE_CLK_ENABLE();
#endif
#ifdef GPIOF
    __HAL_RCC_GPIOF_CLK_ENABLE();
#endif
#ifdef GPIOG
    __HAL_RCC_GPIOG_CLK_ENABLE();
#endif
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

#ifdef LED_PIN
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin   = LED_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);

    board_led_write(1);
#endif
}

__attribute__((weak)) void board_dfu_complete(void) {
    NVIC_SystemReset();
}

__attribute__((weak)) bool board_app_valid(void) {
    volatile uint32_t const *app_vector = (volatile uint32_t const *)CONFIG_BOOTUF2_APP_START_ADDR;

    // 2nd word is App entry point (reset)
    if (app_vector[1] < CONFIG_BOOTUF2_APP_START_ADDR || app_vector[1] > CONFIG_BOOTUF2_APP_END_ADDR) {
        return false;
    }

    return true;
}

__attribute__((weak)) void board_app_jump(void) {
    HAL_RCC_DeInit();
    HAL_DeInit();

    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL  = 0;

    // Disable all Interrupts
    for (int i = 0; i < BOARD_ARRAY_SIZE(NVIC->ICER); i++) {
        NVIC->ICER[i] = 0xFFFFFFFF;
        NVIC->ICPR[i] = 0xFFFFFFFF;
    }

    volatile uint32_t const *app_vector = (volatile uint32_t const *)CONFIG_BOOTUF2_APP_START_ADDR;

    /* switch exception handlers to the application */
    SCB->VTOR = (uint32_t)CONFIG_BOOTUF2_APP_START_ADDR;

    __set_CONTROL(0);
    __set_MSP(app_vector[0]);
    asm("bx %0" ::"r"(app_vector[1]));
}

__attribute__((weak)) void board_user_task_process(void) {
}

//--------------------------------------------------------------------+
// LED
//--------------------------------------------------------------------+
void board_led_write(uint32_t state) {
    (void)state;
#ifdef LED_PIN
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, state ? LED_STATE_ON : (1 - LED_STATE_ON));
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
