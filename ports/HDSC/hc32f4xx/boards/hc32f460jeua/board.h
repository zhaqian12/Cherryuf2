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

#include "hc32_ll.h"

// clang-format off
//--------------------------------------------------------------------+
// LED
//--------------------------------------------------------------------+
#define LED_PORT              GPIO_PORT_A
#define LED_PIN               GPIO_PIN_08
#define LED_STATE_ON          0

//--------------------------------------------------------------------+
// FLASH 
//--------------------------------------------------------------------+
#define BOARD_SECTOR_SIZE     8192U
#define BOARD_SECTOR_COUNT    64
#define BOARD_FLASH_SIZE      (BOARD_SECTOR_SIZE * BOARD_SECTOR_COUNT)

//--------------------------------------------------------------------+
// USB UF2 
//--------------------------------------------------------------------+
#define USBD_VID              0x00AA
#define USBD_PID              0xAAFF
#define USB_MANUFACTURER      "HDSC"
#define USB_PRODUCT           "HC32F460"

#define UF2_PRODUCT_NAME      USB_MANUFACTURER " " USB_PRODUCT
#define UF2_BOARD_ID          "HC32F460xE"
#define UF2_VOLUME_LABEL      "CherryUF2"
#define UF2_INDEX_URL         "https://www.xhsc.com.cn/Productlist/info.aspx?itemid=1849"

// clang-format on
//--------------------------------------------------------------------+
// CLOCK
//--------------------------------------------------------------------+
static inline void clock_init(void) {
    stc_clock_xtal_init_t stcXtalInit;
    stc_clock_pll_init_t  stcMpllInit;

    /* Set bus clk div. */
    CLK_SetClockDiv(CLK_BUS_CLK_ALL, (CLK_HCLK_DIV1 | CLK_EXCLK_DIV2 | CLK_PCLK0_DIV1 |
                                      CLK_PCLK1_DIV2 | CLK_PCLK2_DIV4 | CLK_PCLK3_DIV4 | CLK_PCLK4_DIV2));

    GPIO_AnalogCmd(GPIO_PORT_H, GPIO_PIN_01 | GPIO_PIN_00, ENABLE);
    (void)CLK_XtalStructInit(&stcXtalInit);
    /* Config Xtal and enable Xtal */
    stcXtalInit.u8Mode  = CLK_XTAL_MD_OSC;
    stcXtalInit.u8Drv   = CLK_XTAL_DRV_ULOW;
    stcXtalInit.u8State = CLK_XTAL_ON;
    stcXtalInit.u8StableTime = CLK_XTAL_STB_2MS;
    (void)CLK_XtalInit(&stcXtalInit);

    (void)CLK_PLLStructInit(&stcMpllInit);
    /* MPLL config (XTAL / pllmDiv * plln / PllpDiv = 200M). */
    stcMpllInit.PLLCFGR = 0UL;
    stcMpllInit.PLLCFGR_f.PLLM = (XTAL_VALUE / 1000000UL) - 1UL;
    stcMpllInit.PLLCFGR_f.PLLN = 400UL - 1UL;
    stcMpllInit.PLLCFGR_f.PLLP = 2UL - 1UL;
    stcMpllInit.PLLCFGR_f.PLLQ = 2UL - 1UL;
    stcMpllInit.PLLCFGR_f.PLLR = 2UL - 1UL;
    stcMpllInit.u8PLLState = CLK_PLL_ON;
    stcMpllInit.PLLCFGR_f.PLLSRC = CLK_PLL_SRC_XTAL;
    (void)CLK_PLLInit(&stcMpllInit);
    /* Wait MPLL ready. */
    while (SET != CLK_GetStableStatus(CLK_STB_FLAG_PLL)) {
    }

    /* sram init include read/write wait cycle setting */
    SRAM_Init();
    SRAM_SetWaitCycle(SRAM_SRAM_ALL, SRAM_WAIT_CYCLE1, SRAM_WAIT_CYCLE1);
    SRAM_SetWaitCycle(SRAM_SRAMH, SRAM_WAIT_CYCLE0, SRAM_WAIT_CYCLE0);
    /* 3 cycles for 126MHz ~ 200MHz */
    GPIO_SetReadWaitCycle(GPIO_RD_WAIT3);
    /* flash read wait cycle setting */
    EFM_SetWaitCycle(EFM_WAIT_CYCLE5);
    /* Switch driver ability */
    (void)PWC_HighSpeedToHighPerformance();
    /* Switch system clock source to MPLL. */
    CLK_SetSysClockSrc(CLK_SYSCLK_SRC_PLL);
}

static inline void clock_deinit(void)
{
    CLK_SetSysClockSrc(CLK_SYSCLK_SRC_MRC);
    /* Switch driver ability */
    (void)PWC_HighPerformanceToHighSpeed();
    /* Set bus clk div. */
    CLK_SetClockDiv(CLK_BUS_CLK_ALL, (CLK_HCLK_DIV1 | CLK_EXCLK_DIV1 | CLK_PCLK0_DIV1 |
                                      CLK_PCLK1_DIV1 | CLK_PCLK2_DIV1 | CLK_PCLK3_DIV1 | CLK_PCLK4_DIV1));
    CLK_PLLCmd(DISABLE);
    CLK_XtalCmd(DISABLE);
    /* sram init include read/write wait cycle setting */
    SRAM_SetWaitCycle(SRAM_SRAM_ALL, SRAM_WAIT_CYCLE0, SRAM_WAIT_CYCLE0);
    SRAM_SetWaitCycle(SRAM_SRAMH, SRAM_WAIT_CYCLE0, SRAM_WAIT_CYCLE0);
    /* 0 cycles */
    GPIO_SetReadWaitCycle(GPIO_RD_WAIT0);
    /* flash read wait cycle setting */
    EFM_SetWaitCycle(EFM_WAIT_CYCLE0);
}
#endif
