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
static struct usbd_interface intf0;
static struct usbd_interface intf1;

__attribute__((weak)) void board_uf2boot_init(void) {
#if (defined(USB_XFER_USE_FS) || defined(USB_XFER_USE_FS_HS))
    usbd_desc_register(BOOTUF2_BUS_ID_FS, &bootuf2_descriptor);
    usbd_add_interface(BOOTUF2_BUS_ID_FS, usbd_msc_init_intf(BOOTUF2_BUS_ID_FS, &intf0, BOOTUF2_OUT_EP, BOOTUF2_IN_EP));
    usbd_initialize(BOOTUF2_BUS_ID_FS, USB_DEVICE_SPEED_FS, OTGFS1_BASE, usbd_event_handler);
#endif
#if (defined(USB_XFER_USE_HS) || defined(USB_XFER_USE_FS_HS))
    usbd_desc_register(BOOTUF2_BUS_ID_HS, &bootuf2_descriptor);
    usbd_add_interface(BOOTUF2_BUS_ID_HS, usbd_msc_init_intf(BOOTUF2_BUS_ID_HS, &intf1, BOOTUF2_OUT_EP, BOOTUF2_IN_EP));
    usbd_initialize(BOOTUF2_BUS_ID_HS, USB_DEVICE_SPEED_HS, OTGHS_BASE, usbd_event_handler);
#endif
}

__attribute__((weak)) void usb_dc_low_level_init(uint8_t busid) {
    if (busid == BOOTUF2_BUS_ID_FS) {
        crm_pllu_output_set(TRUE);
        while (crm_flag_get(CRM_PLLU_STABLE_FLAG) != SET) {
        }
        crm_usb_clock_source_select(CRM_USB_CLOCK_SOURCE_PLLU);
        crm_periph_clock_enable(CRM_OTGFS1_PERIPH_CLOCK, TRUE);
        nvic_irq_enable(OTGFS1_IRQn, 0, 0);
    } else {
        crm_periph_clock_enable(CRM_OTGHS_PERIPH_CLOCK, TRUE);
        nvic_irq_enable(OTGHS_IRQn, 0, 0);
    }
}

//--------------------------------------------------------------------+
// IRQ Handler
//--------------------------------------------------------------------+
void OTGFS1_IRQHandler(void) {
    extern void USBD_IRQHandler(uint8_t busid);
    USBD_IRQHandler(BOOTUF2_BUS_ID_FS);
}

void OTGHS_IRQHandler(void) {
    extern void USBD_IRQHandler(uint8_t busid);
    USBD_IRQHandler(BOOTUF2_BUS_ID_HS);
}

//--------------------------------------------------------------------+
// dwc2 fifo configuration
//--------------------------------------------------------------------+
static const uint16_t usbd_dwc2_rxfifo_size[2] = {
    128, // OTGFS1
    256, // OTGHS
};

static const uint16_t usbd_dwc2_txfifo_size[2][8] = {
    {24, 20, 20, 20, 20, 20, 0, 0},  // OTGFS1
    {64, 256, 64, 64, 64, 64, 0, 0}, // OTGHS
};

uint16_t usbd_get_dwc2_rxfifo_conf(uint8_t busid) {
    if (busid == BOOTUF2_BUS_ID_FS) {
        return usbd_dwc2_rxfifo_size[0];
    } else {
        return usbd_dwc2_rxfifo_size[1];
    }
}

uint16_t usbd_get_dwc2_txfifo_conf(uint8_t busid, uint8_t fifoid) {
    if (fifoid > 7) {
        return 0;
    }

    if (busid == BOOTUF2_BUS_ID_FS) {
        return usbd_dwc2_txfifo_size[0][fifoid];
    } else {
        return usbd_dwc2_txfifo_size[1][fifoid];
    }
}
