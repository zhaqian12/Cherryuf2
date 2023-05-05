# Cherryuf2
This repo is cross-platform UF2 Bootloader projects for MCUs based on Cherryusb.It is a simple bootloader that forks from [tinyuf2](https://github.com/adafruit/tinyuf2) and migrates the USB stack to [CherryUSB](https://github.com/sakumisu/CherryUSB).
It only supports DFU with MSC.
```
.
├── lib                   # Sources from 3rd party such as cherryusb, uf2 ...
├── sdk                   # Sources from 3rd party such as mcu drivers
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
| Manufacturer | MCU         | Note         |
| :----------  | :---------- | :----------  |
| ArteryTek    | AT32F403A   |              |
| ArteryTek    | AT32F407    |              |
| ArteryTek    | AT32F413    |              |
| ArteryTek    | AT32F415    |              |
| ST           | STM32F072   | Need to copy the vector table from the Flash to the base address of the SRAM at 0x20000000 in app           |
| ST           | STM32F103   |              |
| ST           | STM32G431   |              |
| ST           | STM32L433   |              |
| WCH          | CH32V307    |              |
| WCH          | CH582M      |              |

## Todo List
- [ ] AT32F435/437 
- [ ] AT32WB415
- [ ] HC32F460 
- [X] CH32V307
- [X] CH582
- [ ] CH32F208
- [ ] CH32V208
- [ ] ...

# How to build
```
$ cd ports/ArtertTek/at32f413
$ make BOARD=at32f413cbt7 all
```

# How to clean
```
$ cd ports/ArtertTek/at32f413
$ make BOARD=at32f413cbt7 clean
```