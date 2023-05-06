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
    /* Peripheral clock enable */
    crm_periph_clock_enable(CRM_USB_PERIPH_CLOCK, TRUE);
    /* USB interrupt Init */
    nvic_irq_enable(USBFS_L_CAN1_RX0_IRQn, 0, 0);
}

//--------------------------------------------------------------------+
// Boards api
//--------------------------------------------------------------------+
__attribute__((weak)) void board_init(void) {
    nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

    clock_init();
    system_core_clock_update();

    board_timer_stop();

    crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
    // crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
    // crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
    // crm_periph_clock_enable(CRM_GPIOF_PERIPH_CLOCK, TRUE);
    // crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);

#ifdef LED_PIN
    gpio_init_type gpio_init_struct;

    gpio_default_para_init(&gpio_init_struct);
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_out_type       = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_mode           = GPIO_MODE_OUTPUT;
    gpio_init_struct.gpio_pins           = LED_PIN;
    gpio_init_struct.gpio_pull           = GPIO_PULL_NONE;
    gpio_init(LED_PORT, &gpio_init_struct);

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

    // 2nd word is App entry point (reset)
    if (app_vector[1] < BOARD_FLASH_APP_START || app_vector[1] > BOARD_FLASH_APP_START + BOARD_FLASH_SIZE) {
        return false;
    }

    return true;
}

__attribute__((weak)) void board_app_jump(void) {
#ifdef LED_PIN
    gpio_reset(LED_PORT);
#endif

    crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, FALSE);
    //  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, FALSE);
    //  crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, FALSE);
    crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, FALSE);
    //  crm_periph_clock_enable(CRM_GPIOF_PERIPH_CLOCK, FALSE);
    //  crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, FALSE);
    crm_periph_clock_enable(CRM_USB_PERIPH_CLOCK, FALSE);
    crm_reset();

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
    asm("bx %0" ::"r"(app_vector[1]));
}

//--------------------------------------------------------------------+
// LED
//--------------------------------------------------------------------+
void board_led_write(uint32_t state) {
    (void)state;
#ifdef LED_PIN
    gpio_bits_write(LED_PORT, LED_PIN, state ? LED_STATE_ON : (1 - LED_STATE_ON));
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
