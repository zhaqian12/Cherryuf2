/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Ha Thach for Adafruit Industries
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

#ifndef BOARD_H_
#define BOARD_H_

#include "at32f413.h"

//--------------------------------------------------------------------+
// LED
//--------------------------------------------------------------------+
#define LED_PORT              GPIOA
#define LED_PIN               GPIO_PINS_1
#define LED_STATE_ON          0

//--------------------------------------------------------------------+
// FLASH 
//--------------------------------------------------------------------+
#define BOARD_SECTOR_SIZE     1024U
#define BOARD_SECTOR_COUNT    128
#define BOARD_FLASH_SIZE      (BOARD_SECTOR_SIZE * BOARD_SECTOR_COUNT)

//--------------------------------------------------------------------+
// USB UF2 
//--------------------------------------------------------------------+
#define USBD_VID              0x00AA
#define USBD_PID              0xAAFF
#define USB_MANUFACTURER      "Zhaqian"
#define USB_PRODUCT           "AT32F413"

#define UF2_PRODUCT_NAME      USB_MANUFACTURER " " USB_PRODUCT
#define UF2_BOARD_ID          "AT32F413xB"
#define UF2_VOLUME_LABEL      "Zhaqian"
#define UF2_INDEX_URL         "https://www.arterytek.com/cn/index.jsp"

//--------------------------------------------------------------------+
// CLOCK
//--------------------------------------------------------------------+
static inline void clock_init(void)
{
  crm_reset();

  crm_clock_source_enable(CRM_CLOCK_SOURCE_HEXT, TRUE);

  while(crm_hext_stable_wait() == ERROR)
  {
  }

  crm_pll_config(CRM_PLL_SOURCE_HEXT, CRM_PLL_MULT_24, CRM_PLL_OUTPUT_RANGE_GT72MHZ);
  crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);
  while(crm_flag_get(CRM_PLL_STABLE_FLAG) != SET)
  {
  }

  crm_ahb_div_set(CRM_AHB_DIV_1);
  crm_apb2_div_set(CRM_APB2_DIV_2);
  crm_apb1_div_set(CRM_APB1_DIV_2);
  crm_usb_clock_div_set(CRM_USB_DIV_4);
  crm_auto_step_mode_enable(TRUE);
  crm_sysclk_switch(CRM_SCLK_PLL);
  while(crm_sysclk_switch_status_get() != CRM_SCLK_PLL)
  {
  }
  crm_auto_step_mode_enable(FALSE);
  crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_BPR_PERIPH_CLOCK, TRUE);
}
#endif
