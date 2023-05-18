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
    stc_gpio_init_t stcGpioCfg;
    stc_clock_pllx_init_t stcUpllInit;

    (void)CLK_PLLxStructInit(&stcUpllInit);
    stcUpllInit.u8PLLState = CLK_PLLX_ON;
    stcUpllInit.PLLCFGR = 0UL;
    stcUpllInit.PLLCFGR_f.PLLM = (XTAL_VALUE / 1000000UL) - 1UL;
    stcUpllInit.PLLCFGR_f.PLLN = (336UL - 1UL);
    stcUpllInit.PLLCFGR_f.PLLR = (7UL  - 1UL);
    stcUpllInit.PLLCFGR_f.PLLQ = (7UL - 1UL);
    stcUpllInit.PLLCFGR_f.PLLP = (7UL - 1UL);
    (void)CLK_PLLxInit(&stcUpllInit);

    CLK_SetUSBClockSrc(CLK_USBCLK_PLLXP);

    (void)GPIO_StructInit(&stcGpioCfg);
    stcGpioCfg.u16PinAttr = PIN_ATTR_ANALOG;
    (void)GPIO_Init(GPIO_PORT_A, GPIO_PIN_11, &stcGpioCfg);
    (void)GPIO_Init(GPIO_PORT_A, GPIO_PIN_12, &stcGpioCfg);
    GPIO_SetFunc(GPIO_PORT_A, GPIO_PIN_09, GPIO_FUNC_10);
    FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_USBFS, ENABLE);

    stc_irq_signin_config_t stcIrqRegiConf;
    stcIrqRegiConf.enIRQn = INT024_IRQn;
    stcIrqRegiConf.enIntSrc = INT_SRC_USBFS_GLB;
    stcIrqRegiConf.pfnCallback = &USB_IRQ_Handler;
    (void)INTC_IrqSignIn(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIO_15);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
}

//--------------------------------------------------------------------+
// Boards api
//--------------------------------------------------------------------+

__attribute__((weak)) void board_init(void) {

    LL_PERIPH_WE(LL_PERIPH_GPIO | LL_PERIPH_EFM | LL_PERIPH_FCG | 
                LL_PERIPH_PWC_CLK_RMU | LL_PERIPH_SRAM);

    EFM_FWMC_Cmd(ENABLE);
    
    clock_init();

    SystemCoreClockUpdate();

    board_timer_stop();

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

__attribute__((weak)) void board_usb_process(void) {
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

    GPIO_DeInit();

    FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_USBFS, DISABLE);

    EFM_FWMC_Cmd(DISABLE);

    clock_deinit();

    LL_PERIPH_WP(LL_PERIPH_EFM | LL_PERIPH_FCG | LL_PERIPH_SRAM);
    
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

int board_uart_write(void const *buf, int len) {
    (void)buf;
    (void)len;
    return 0;
}

void _init(void) {}
