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
// CherryUSB LLD
//--------------------------------------------------------------------+
extern const uint8_t msc_descriptor[];

static struct usbd_interface intf0;

void board_msc_init(void) {
    usbd_desc_register(0, msc_descriptor);
    usbd_add_interface(0, usbd_msc_init_intf(0, &intf0, MSC_OUT_EP, MSC_IN_EP));

    usbd_initialize(0, CM_USBFS_BASE, usbd_event_handler);
}

__attribute__((weak)) void usb_dc_low_level_init(void) {
    stc_gpio_init_t       stcGpioCfg;
    stc_clock_pllx_init_t stcUpllInit;

    (void)CLK_PLLxStructInit(&stcUpllInit);
    stcUpllInit.u8PLLState     = CLK_PLLX_ON;
    stcUpllInit.PLLCFGR        = 0UL;
    stcUpllInit.PLLCFGR_f.PLLM = (XTAL_VALUE / 1000000UL) - 1UL;
    stcUpllInit.PLLCFGR_f.PLLN = (336UL - 1UL);
    stcUpllInit.PLLCFGR_f.PLLR = (7UL - 1UL);
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
    stcIrqRegiConf.enIRQn      = INT024_IRQn;
    stcIrqRegiConf.enIntSrc    = INT_SRC_USBFS_GLB;
    stcIrqRegiConf.pfnCallback = &USB_IRQ_Handler;
    (void)INTC_IrqSignIn(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIO_15);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
}

//--------------------------------------------------------------------+
// IRQ Handler
//--------------------------------------------------------------------+
void USB_IRQ_Handler(void) {
    extern void USBD_IRQHandler(uint8_t busid);
    USBD_IRQHandler(0);
}
