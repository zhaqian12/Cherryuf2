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
// FLASH
//--------------------------------------------------------------------+
#define FLASH_BASE_ADDR 0x08000000UL

#define SECTOR_SIZE BOARD_SECTOR_SIZE
#define SECTOR_COUNT BOARD_SECTOR_COUNT

static uint8_t erased_sectors[SECTOR_COUNT] = {0};

static bool is_blank(uint32_t addr, uint32_t size) {
    for (uint32_t i = 0; i < size; i += sizeof(uint32_t)) {
        if (*(uint32_t *)(addr + i) != 0xffffffff) {
            return false;
        }
    }
    return true;
}

static bool flash_erase(uint32_t addr) {
    // starting address from 0x08000000
    uint32_t sector_addr = FLASH_BASE_ADDR;
    bool     erased      = false;

    uint32_t sector = 0;
    uint32_t size   = 0;
    (void)sector;
    for (uint32_t i = 0; i < SECTOR_COUNT; i++) {
        size = SECTOR_SIZE;
        if (sector_addr + size > addr) {
            sector            = i;
            erased            = erased_sectors[i];
            erased_sectors[i] = 1; // don't erase anymore - we will continue writing here!
            break;
        }
        sector_addr += size;
    }

    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
    if (!erased && !is_blank(sector_addr, size)) {
        FLASH_EraseInitTypeDef EraseInitStruct = {0};
        EraseInitStruct.TypeErase              = TYPEERASE_PAGES;
        EraseInitStruct.Banks                  = FLASH_BANK_1;
        EraseInitStruct.Page                   = sector;
        EraseInitStruct.NbPages                = 1;

        uint32_t SectorError = 0;
        HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError);
        FLASH_WaitForLastOperation(HAL_MAX_DELAY);
    }

    return true;
}

static void flash_write(uint32_t dst, const uint8_t *src, int len) {
    flash_erase(dst);

    for (int i = 0; i < len; i += 8) {
        uint64_t data = *((uint64_t *)((void *)(src + i)));
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, dst + i, data) != HAL_OK) {
            break;
        }

        if (FLASH_WaitForLastOperation(HAL_MAX_DELAY) != HAL_OK) {
            return;
        }
    }

    // verify contents
    if (memcmp((void *)dst, src, len) != 0) {
    }
}

//--------------------------------------------------------------------+
//
//--------------------------------------------------------------------+
__attribute__((weak)) void board_flash_init(void) {}

__attribute__((weak)) uint32_t board_flash_size(void) {
    return BOARD_FLASH_SIZE;
}

void board_flash_read(uint32_t addr, void *buffer, uint32_t len) {
    memcpy(buffer, (void *)addr, len);
}

__attribute__((weak)) void board_flash_flush(void) {}

__attribute__((weak)) void board_flash_write(uint32_t addr, void const *data, uint32_t len) {
    HAL_FLASH_Unlock();
    flash_write(addr, data, len);
    HAL_FLASH_Lock();
}

__attribute__((weak)) void board_flash_erase_app(void) {}

#ifdef CHERRYUF2_SELF_UPDATE
void board_self_update(const uint8_t *bootloader_bin, uint32_t bootloader_len) {
    (void)bootloader_bin;
    (void)bootloader_len;
}
#endif
