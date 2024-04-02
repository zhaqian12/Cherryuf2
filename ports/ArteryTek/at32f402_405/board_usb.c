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
    usbd_initialize(0, OTGHS_BASE, usbd_event_handler);
#else
    usbd_initialize(0, OTGFS1_BASE, usbd_event_handler);
#endif
}

__attribute__((weak)) void usb_dc_low_level_init(void) {
#ifdef CONFIG_USB_HS
    crm_periph_clock_enable(CRM_OTGHS_PERIPH_CLOCK, TRUE);
    nvic_irq_enable(OTGHS_IRQn, 0, 0);
#else
    crm_pllu_output_set(TRUE);
    while (crm_flag_get(CRM_PLLU_STABLE_FLAG) != SET) {
    }
    crm_usb_clock_source_select(CRM_USB_CLOCK_SOURCE_PLLU);
    crm_periph_clock_enable(CRM_OTGFS1_PERIPH_CLOCK, TRUE);
    nvic_irq_enable(OTGFS1_IRQn, 0, 0);
#endif
}

//--------------------------------------------------------------------+
// IRQ Handler
//--------------------------------------------------------------------+
#ifdef CONFIG_USB_HS
void OTGHS_IRQHandler(void) {
    extern void USBD_IRQHandler(uint8_t busid);
    USBD_IRQHandler(0);
}
#else
void OTGFS1_IRQHandler(void) {
    extern void USBD_IRQHandler(uint8_t busid);
    USBD_IRQHandler(0);
}
#endif




