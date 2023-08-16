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

#ifndef BOARD_H_
#define BOARD_H_

#include "at32f423.h"

// clang-format off
//--------------------------------------------------------------------+
// LED
//--------------------------------------------------------------------+
#define LED_PORT              GPIOA
#define LED_PIN               GPIO_PINS_1
#define LED_STATE_ON          0

//--------------------------------------------------------------------+
// FLASH 
//--------------------------------------------------------------------+
#define BOARD_SECTOR_SIZE     2048U
#define BOARD_SECTOR_COUNT    128
#define BOARD_FLASH_SIZE      (BOARD_SECTOR_SIZE * BOARD_SECTOR_COUNT)

//--------------------------------------------------------------------+
// USB UF2 
//--------------------------------------------------------------------+
#define USBD_VID              0x00AA
#define USBD_PID              0xAAFF
#define USB_MANUFACTURER      "ArteryTek"
#define USB_PRODUCT           "AT32F423"

#define UF2_PRODUCT_NAME      USB_MANUFACTURER " " USB_PRODUCT
#define UF2_BOARD_ID          "AT32F423xC"
#define UF2_VOLUME_LABEL      "CherryUF2"
#define UF2_INDEX_URL         "https://www.arterytek.com/cn/product/AT32F423.jsp"

// clang-format on
//--------------------------------------------------------------------+
// CLOCK
//--------------------------------------------------------------------+
static inline void clock_init(void) {
  crm_reset();
  flash_psr_set(FLASH_WAIT_CYCLE_4);
  crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);
  pwc_ldo_output_voltage_set(PWC_LDO_OUTPUT_1V3);
  crm_clock_source_enable(CRM_CLOCK_SOURCE_HICK, TRUE);
  while(crm_flag_get(CRM_HICK_STABLE_FLAG) != SET)
  {
  }
  crm_pll_config(CRM_PLL_SOURCE_HICK, 72, 1, CRM_PLL_FR_2);
  crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);
  while(crm_flag_get(CRM_PLL_STABLE_FLAG) != SET)
  {
  }
  crm_ahb_div_set(CRM_AHB_DIV_1);
  crm_apb2_div_set(CRM_APB2_DIV_1);
  crm_apb1_div_set(CRM_APB1_DIV_2);
  crm_auto_step_mode_enable(TRUE);
  crm_sysclk_switch(CRM_SCLK_PLL);
  while(crm_sysclk_switch_status_get() != CRM_SCLK_PLL)
  {
  }
  crm_auto_step_mode_enable(FALSE);
}
#endif
