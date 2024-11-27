# Cherryuf2
This repo is cross-platform UF2 Bootloader projects for MCUs based on Cherryusb.It is a simple bootloader that forks from [tinyuf2](https://github.com/adafruit/tinyuf2) and migrates the USB stack to [CherryUSB](https://github.com/sakumisu/CherryUSB).
It only supports DFU with MSC.
```
.
├── lib                   # Sources from 3rd party such as cherryusb, uf2 ...
├── SDK                   # Sources from 3rd party such as mcu drivers
├── bootloader            # Bin for bootloader of this repo
├── ports                 # Port/family specific sources
│   ├── ArtertTek         # MCU manufacturer
│   |   └── at32f413      # MCU Series
|   |       └── boards    # Board specific sources
│   |       └── Makefile  # Makefile for this port  
│   |       └── port.mk   # Makefile for this port         
│   └── ST                # MCU manufacturer
│       └── stm32f1xx     # MCU Series
├── src                   # Cross-platform bootloader sources files
```

# Support Devices
### ArteryTek 
| MCU       | Note | SDK                                                                                  |
| :-------- | :--- | :----------------------------------------------------------------------------------- |
| AT32F402  |      | [AT32F402_405_SDK](https://github.com/ArteryTek/AT32F402_405_Firmware_Library.git)   |
| AT32F403A |      | [AT32F403A_407_SDK](https://github.com/ArteryTek/AT32F403A_407_Firmware_Library.git) |
| AT32F405  |      | [AT32F402_405_SDK](https://github.com/ArteryTek/AT32F402_405_Firmware_Library.git)   |
| AT32F407  |      | [AT32F403A_407_SDK](https://github.com/ArteryTek/AT32F403A_407_Firmware_Library.git) |
| AT32F413  |      | [AT32F413_SDK](https://github.com/ArteryTek/AT32F413_Firmware_Library.git)           |
| AT32F415  |      | [AT32F415_SDK](https://github.com/ArteryTek/AT32F415_Firmware_Library.git)           |
| AT32F423  |      | [AT32F423_SDK](https://github.com/ArteryTek/AT32F423_Firmware_Library.git)           |
| AT32F425  |      | [AT32F425_SDK](https://github.com/ArteryTek/AT32F425_Firmware_Library.git)           |
| AT32F435  |      | [AT32F435_437_SDK](https://github.com/ArteryTek/AT32F435_437_Firmware_Library.git)   |
| AT32F437  |      | [AT32F435_437_SDK](https://github.com/ArteryTek/AT32F435_437_Firmware_Library.git)   |
| AT32WB415 |      | [AT32WB415_SDK](https://github.com/ArteryTek/AT32WB415_Firmware_Library.git)         |
---
### STMicroelectronics
| MCU       | Note                                           | SDK                                                                                                                                                               |
| :-------- | :--------------------------------------------- | :---------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| STM32F072 | Need to redirect vectors in app or bootloader. | [STM32F0xx_Hal](https://github.com/STMicroelectronics/stm32f0xx_hal_driver.git) <br> [STM32F0xx_CMSIS](https://github.com/STMicroelectronics/cmsis_device_f0.git) |
| STM32F103 |                                                | [STM32F1xx_Hal](https://github.com/STMicroelectronics/stm32f1xx_hal_driver.git) <br> [STM32F1xx_CMSIS](https://github.com/STMicroelectronics/cmsis_device_f1.git) |
|STM32F401 |                                                                             | [STM32F4xx_Hal](https://github.com/STMicroelectronics/stm32f4xx_hal_driver.git) <br> [STM32F4xx_CMSIS](https://github.com/STMicroelectronics/cmsis_device_f4.git) 
|STM32F407 |                                                                             | [STM32F4xx_Hal](https://github.com/STMicroelectronics/stm32f4xx_hal_driver.git) <br> [STM32F4xx_CMSIS](https://github.com/STMicroelectronics/cmsis_device_f4.git) 
|STM32F411 |                                                                             | [STM32F4xx_Hal](https://github.com/STMicroelectronics/stm32f4xx_hal_driver.git) <br> [STM32F4xx_CMSIS](https://github.com/STMicroelectronics/cmsis_device_f4.git) 
|STM32G431 |                                                                             | [STM32G4xx_Hal](https://github.com/STMicroelectronics/stm32g4xx_hal_driver.git) <br> [STM32G4xx_CMSIS](https://github.com/STMicroelectronics/cmsis_device_g4.git) 
|STM32L433 |                                                                             | [STM32L4xx_Hal](https://github.com/STMicroelectronics/stm32l4xx_hal_driver.git) <br> [STM32L4xx_CMSIS](https://github.com/STMicroelectronics/cmsis_device_l4.git) 


---
## Todo List
- [ ] Arterytek
- [ ] HPM
- [ ] WCH
- [ ] ST

# How to build
- Download the SDK of mcu.
- 
    ```
    $ cd ports/ArteryTek/at32f413
    $ make BOARD=at32f413cbt7 all
    ```

# How to clean
```
$ cd ports/ArteryTek/at32f413
$ make BOARD=at32f413cbt7 clean
```

# Changelog
- 2024.4 Update to the latest CherryUSB v1.2.0.
- 2024.11 Update to the latest CherryUSB and bootuf2.