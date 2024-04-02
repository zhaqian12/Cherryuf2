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
// static struct usbd_interface intf1;

void board_msc_init(void) {
#ifdef AT32_USE_OTGFS1
    usbd_desc_register(0, msc_descriptor);
    usbd_add_interface(0, usbd_msc_init_intf(0, &intf0, MSC_OUT_EP, MSC_IN_EP));

    usbd_initialize(0, OTGFS1_BASE, usbd_event_handler);
#endif
// #ifdef AT32_USE_OTGFS2                                       // wait for DWC2 multiports support
//     usbd_desc_register(1, msc_descriptor);
//     usbd_add_interface(1, usbd_msc_init_intf(1, &intf1, MSC_OUT_EP, MSC_IN_EP));

//     usbd_initialize(1, OTGFS2_BASE, usbd_event_handler);
// #endif
#ifdef AT32_USE_OTGFS2
    usbd_desc_register(0, msc_descriptor);
    usbd_add_interface(0, usbd_msc_init_intf(0, &intf0, MSC_OUT_EP, MSC_IN_EP));

    usbd_initialize(0, OTGFS2_BASE, usbd_event_handler);
#endif
}

__attribute__((weak)) void usb_dc_low_level_init(void) {
    gpio_init_type gpio_init_struct;

    gpio_default_para_init(&gpio_init_struct);

    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_out_type       = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_mode           = GPIO_MODE_MUX;
    gpio_init_struct.gpio_pull           = GPIO_PULL_NONE;

#ifdef AT32_USE_OTGFS1
    gpio_init_struct.gpio_pins = GPIO_PINS_11 | GPIO_PINS_12;
    gpio_init(GPIOA, &gpio_init_struct);

    gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE11, GPIO_MUX_10);
    gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE12, GPIO_MUX_10);

#    ifdef OTG_SOF_OUTPUT_ENABLE
    gpio_init_struct.gpio_pins = GPIO_PINS_8;
    gpio_init(GPIOA, &gpio_init_struct);
    gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE8, GPIO_MUX_10);
#    endif

#    ifndef OTG_VBUS_IGNORE
    gpio_init_struct.gpio_pins = GPIO_PINS_9;
    gpio_init_struct.gpio_pull = GPIO_PULL_DOWN;
    gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE9, GPIO_MUX_10);
    gpio_init(GPIOA, &gpio_init_struct);
#    endif

    crm_periph_clock_enable(CRM_OTGFS1_PERIPH_CLOCK, TRUE);
    nvic_irq_enable(OTGFS1_IRQn, 0, 0);
#endif

#ifdef AT32_USE_OTGFS2
    gpio_init_struct.gpio_pins = GPIO_PINS_14 | GPIO_PINS_15;
    gpio_init(GPIOB, &gpio_init_struct);

    gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE14, GPIO_MUX_12);
    gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE15, GPIO_MUX_12);

#    ifdef OTG_SOF_OUTPUT_ENABLE
    gpio_init_struct.gpio_pins = GPIO_PINS_4;
    gpio_init(GPIOA, &gpio_init_struct);
    gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE4, GPIO_MUX_12);
#    endif

#    ifndef OTG_VBUS_IGNORE
    gpio_init_struct.gpio_pins = GPIO_PINS_13;
    gpio_init_struct.gpio_pull = GPIO_PULL_DOWN;
    gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE13, GPIO_MUX_12);
    gpio_init(GPIOB, &gpio_init_struct);
#    endif

    crm_periph_clock_enable(CRM_OTGFS2_PERIPH_CLOCK, TRUE);
    nvic_irq_enable(OTGFS2_IRQn, 0, 0);
#endif
}

//--------------------------------------------------------------------+
// IRQ Handler
//--------------------------------------------------------------------+
void OTGFS1_IRQHandler(void) {
    extern void USBD_IRQHandler(uint8_t busid);
    USBD_IRQHandler(0);
}

void OTGFS2_IRQHandler(void) {
    extern void USBD_IRQHandler(uint8_t busid);
    USBD_IRQHandler(0);
}
