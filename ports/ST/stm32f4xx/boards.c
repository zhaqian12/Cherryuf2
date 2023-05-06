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
    GPIO_InitTypeDef GPIO_InitStruct;

    // USB Pin Init
    // PA9- VUSB, PA11- DM, PA12- DP

    /* Configure DM DP Pins */
    GPIO_InitStruct.Pin       = GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Configure VBUS Pin */
#ifdef CONFIG_DWC2_VBUS_SENSING_ENABLE
    GPIO_InitStruct.Pin  = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#endif

    /* Peripheral clock enable */
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();

    /* Peripheral interrupt init */
    HAL_NVIC_SetPriority(OTG_FS_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
}

//--------------------------------------------------------------------+
// Boards api
//--------------------------------------------------------------------+
__attribute__((weak)) void board_init(void) {
    clock_init();
    SystemCoreClockUpdate();

    board_timer_stop();

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

__attribute__((weak)) void board_usb_process(void) {
    // todo
}

__attribute__((weak)) bool board_app_valid(void) {
    volatile uint32_t const *app_vector = (volatile uint32_t const *)BOARD_FLASH_APP_START;

    // 1st word is stack pointer (must be in SRAM region)
    if ((app_vector[0] & 0xff000003) != 0x20000000) return false;

    // 2nd word is App entry point (reset)
    if (app_vector[1] < BOARD_FLASH_APP_START || app_vector[1] > BOARD_FLASH_APP_START + BOARD_FLASH_SIZE) {
        return false;
    }

    return true;
}

__attribute__((weak)) void board_app_jump(void) {
#ifdef LED_PIN
    HAL_GPIO_DeInit(LED_PORT, LED_PIN);
#endif

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_12 | GPIO_PIN_11 | GPIO_PIN_9);

    __HAL_RCC_GPIOA_CLK_DISABLE();
    __HAL_RCC_GPIOB_CLK_DISABLE();
    __HAL_RCC_GPIOC_CLK_DISABLE();
#ifdef GPIOD
    __HAL_RCC_GPIOD_CLK_DISABLE();
#endif
#ifdef GPIOE
    __HAL_RCC_GPIOE_CLK_DISABLE();
#endif
#ifdef GPIOF
    __HAL_RCC_GPIOF_CLK_DISABLE();
#endif
#ifdef GPIOG
    __HAL_RCC_GPIOG_CLK_DISABLE();
#endif
    __HAL_RCC_GPIOH_CLK_DISABLE();
    __HAL_RCC_USB_OTG_FS_CLK_DISABLE();

    HAL_RCC_DeInit();
    HAL_DeInit();

    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL  = 0;

    // Disable all Interrupts
    NVIC->ICER[0] = 0xFFFFFFFF;
    NVIC->ICER[1] = 0xFFFFFFFF;
    NVIC->ICER[2] = 0xFFFFFFFF;
    NVIC->ICER[3] = 0xFFFFFFFF;

    volatile uint32_t const *app_vector = (volatile uint32_t const *)BOARD_FLASH_APP_START;

    /* switch exception handlers to the application */
    SCB->VTOR = (uint32_t)BOARD_FLASH_APP_START;

    // Set stack pointer
    __set_MSP(app_vector[0]);
    __set_PSP(app_vector[0]);

    // Jump to Application Entry
    asm("bx %0" ::"r"(app_vector[1]));
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

int board_uart_write(void const *buf, int len) {
    (void)buf;
    (void)len;
    return 0;
}

void _init(void) {}
