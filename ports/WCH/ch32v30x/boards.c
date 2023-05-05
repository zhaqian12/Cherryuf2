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
void usb_dc_low_level_init(void)
{
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
// Boards api
//--------------------------------------------------------------------+
void board_init(void)
{
  Delay_Init();

  clock_init();
  SystemCoreClockUpdate();

  board_timer_stop();

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

#ifdef LED_PIN
  GPIO_InitTypeDef GPIO_InitStructure = {0};

  GPIO_InitStructure.GPIO_Pin = LED_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LED_PORT, &GPIO_InitStructure);
#endif
}

void board_dfu_complete(void)
{
  NVIC_SystemReset();
}

void board_usb_process(void)
{
  // todo
}

bool board_app_valid(void)
{
  // need to improve
  const uint32_t val = *(volatile uint32_t const *)BOARD_FLASH_APP_START;

  return (val != 0xe339e339);
}

void board_app_jump(void)
{
#ifdef LED_PIN
  GPIO_DeInit(LED_PORT);
#endif

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, DISABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, DISABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, DISABLE);
  RCC_USBHSPHYPLLALIVEcmd(DISABLE);
#ifdef CONFIG_USB_HS
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_USBHS, DISABLE);
  NVIC_DisableIRQ(USBHS_IRQn);
#else
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_OTG_FS, DISABLE);
  NVIC_DisableIRQ(OTG_FS_IRQn);
#endif

  NVIC_DisableIRQ(SysTicK_IRQn);
  SysTick->CTLR = 0;
  SysTick->SR = 0;
  SysTick->CNT = 0;
  SysTick->CMP = 0;

  NVIC_EnableIRQ(Software_IRQn);
  NVIC_SetPendingIRQ(Software_IRQn);
}

//--------------------------------------------------------------------+
// LED
//--------------------------------------------------------------------+
void board_led_write(uint32_t state)
{
  GPIO_WriteBit(LED_PORT, LED_PIN, state ? LED_STATE_ON : (1 - LED_STATE_ON));
}

//--------------------------------------------------------------------+
// Timer
//--------------------------------------------------------------------+
uint32_t SysTick_Config(uint32_t ticks)
{
  NVIC_EnableIRQ(SysTicK_IRQn);
  SysTick->CTLR = 0;
  SysTick->SR = 0;
  SysTick->CNT = 0;
  SysTick->CMP = ticks - 1;
  SysTick->CTLR = 0xF;
}

void board_timer_start(uint32_t ms)
{
  SysTick_Config((SystemCoreClock / 1000) * ms);
}

void board_timer_stop(void)
{
  SysTick->CTLR &= ~(1 << 0);
}

void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void SysTick_Handler(void)
{
  SysTick->SR = 0;
  board_timer_handler();
}

int board_uart_write(void const *buf, int len)
{
  (void)buf;
  (void)len;
  return 0;
}

void _init(void)
{
}

//--------------------------------------------------------------------+
// Softwate Interrupt
//--------------------------------------------------------------------+
void SW_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void SW_Handler(void)
{
  // need to improve
  __asm("li  a6, 0x5000"); 
  __asm("jr  a6");

  while (1);
}