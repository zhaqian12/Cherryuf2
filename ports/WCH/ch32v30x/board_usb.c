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
#ifdef CONFIG_USB_HS
    usbd_initialize(0, USBFS_BASE, usbd_event_handler);
#else
    usbd_initialize(0, USBHS_BASE, usbd_event_handler);
#endif
}

__attribute__((weak)) void usb_dc_low_level_init(void) {
    RCC_USBCLK48MConfig(RCC_USBCLK48MCLKSource_USBPHY);
    RCC_USBHSPLLCLKConfig(RCC_HSBHSPLLCLKSource_HSE);
    RCC_USBHSConfig(RCC_USBPLL_Div2);
    RCC_USBHSPLLCKREFCLKConfig(RCC_USBHSPLLCKREFCLK_4M);
    RCC_USBHSPHYPLLALIVEcmd(ENABLE);
#ifdef CONFIG_USB_HS
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_USBHS, ENABLE);
    NVIC_EnableIRQ(USBHS_IRQn);
#else
    // EXTEN->EXTEN_CTR |= EXTEN_USBD_PU_EN;
    NVIC_EnableIRQ(OTG_FS_IRQn);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_OTG_FS, ENABLE);
#endif

    Delay_Us(100);
}

//--------------------------------------------------------------------+
// IRQ Handler
//--------------------------------------------------------------------+
// #ifdef CONFIG_USB_HS
// void USBHS_IRQHandler(void) {
//     extern void USBD_IRQHandler(uint8_t busid);
//     USBD_IRQHandler(0);
// }
// #else
// void OTG_FS_IRQHandler(void) {
//     extern void USBD_IRQHandler(uint8_t busid);
//     USBD_IRQHandler(0);
// }
// #endif