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
#ifdef AT32_USE_OTGFS1
    usbd_desc_register(BOOTUF2_BUS_ID_FS, &bootuf2_descriptor);
    usbd_add_interface(BOOTUF2_BUS_ID_FS, usbd_msc_init_intf(BOOTUF2_BUS_ID_FS, &intf0, BOOTUF2_OUT_EP, BOOTUF2_IN_EP));

    usbd_initialize(BOOTUF2_BUS_ID_FS, USB_DEVICE_SPEED_FS, OTGFS1_BASE, usbd_event_handler);
#endif
#ifdef AT32_USE_OTGFS2
    usbd_desc_register(BOOTUF2_BUS_ID_FS2, &bootuf2_descriptor);
    usbd_add_interface(BOOTUF2_BUS_ID_FS2, usbd_msc_init_intf(BOOTUF2_BUS_ID_FS2, &intf1, BOOTUF2_OUT_EP, BOOTUF2_IN_EP));

    usbd_initialize(BOOTUF2_BUS_ID_FS2, USB_DEVICE_SPEED_FS, OTGFS2_BASE, usbd_event_handler);
#endif
}

__attribute__((weak)) void usb_dc_low_level_init(uint8_t busid) {
    gpio_init_type gpio_init_struct;

    gpio_default_para_init(&gpio_init_struct);

    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_out_type       = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_mode           = GPIO_MODE_MUX;
    gpio_init_struct.gpio_pull           = GPIO_PULL_NONE;

    if (busid == BOOTUF2_BUS_ID_FS) {
        gpio_init_struct.gpio_pins = GPIO_PINS_11 | GPIO_PINS_12;
        gpio_init(GPIOA, &gpio_init_struct);

        gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE11, GPIO_MUX_10);
        gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE12, GPIO_MUX_10);

#ifdef OTG_SOF_OUTPUT_ENABLE
        gpio_init_struct.gpio_pins = GPIO_PINS_8;
        gpio_init(GPIOA, &gpio_init_struct);
        gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE8, GPIO_MUX_10);
#endif

#ifndef OTG_VBUS_IGNORE
        gpio_init_struct.gpio_pins = GPIO_PINS_9;
        gpio_init_struct.gpio_pull = GPIO_PULL_DOWN;
        gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE9, GPIO_MUX_10);
        gpio_init(GPIOA, &gpio_init_struct);
#endif

        crm_periph_clock_enable(CRM_OTGFS1_PERIPH_CLOCK, TRUE);
        nvic_irq_enable(OTGFS1_IRQn, 0, 0);
    } else if (busid == BOOTUF2_BUS_ID_FS2) {
        gpio_init_struct.gpio_pins = GPIO_PINS_14 | GPIO_PINS_15;
        gpio_init(GPIOB, &gpio_init_struct);

        gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE14, GPIO_MUX_12);
        gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE15, GPIO_MUX_12);

#ifdef OTG_SOF_OUTPUT_ENABLE
        gpio_init_struct.gpio_pins = GPIO_PINS_4;
        gpio_init(GPIOA, &gpio_init_struct);
        gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE4, GPIO_MUX_12);
#endif

#ifndef OTG_VBUS_IGNORE
        gpio_init_struct.gpio_pins = GPIO_PINS_13;
        gpio_init_struct.gpio_pull = GPIO_PULL_DOWN;
        gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE13, GPIO_MUX_12);
        gpio_init(GPIOB, &gpio_init_struct);
#endif

        crm_periph_clock_enable(CRM_OTGFS2_PERIPH_CLOCK, TRUE);
        nvic_irq_enable(OTGFS2_IRQn, 0, 0);
    }
}

//--------------------------------------------------------------------+
// IRQ Handler
//--------------------------------------------------------------------+
void OTGFS1_IRQHandler(void) {
    extern void USBD_IRQHandler(uint8_t busid);
    USBD_IRQHandler(BOOTUF2_BUS_ID_FS);
}

void OTGFS2_IRQHandler(void) {
    extern void USBD_IRQHandler(uint8_t busid);
    USBD_IRQHandler(BOOTUF2_BUS_ID_FS2);
}

//--------------------------------------------------------------------+
// dwc2 fifo configuration
//--------------------------------------------------------------------+
static const uint16_t usbd_dwc2_rxfifo_size[2] = {
    128, // OTGFS1
    128, // OTGFS2
};

static const uint16_t usbd_dwc2_txfifo_size[2][8] = {
    {24, 20, 20, 20, 20, 20, 20, 20}, // OTGFS1
    {24, 20, 20, 20, 20, 20, 20, 20}, // OTGFS2
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
