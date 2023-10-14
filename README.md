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
| ArteryTek    | AT32F402    |              |
| ArteryTek    | AT32F403A   |              |
| ArteryTek    | AT32F405    | If you use hs embedded phy, change the init value of the GCCFG register in the CherryUSB port. |
| ArteryTek    | AT32F407    |              |
| ArteryTek    | AT32F413    |              |
| ArteryTek    | AT32F415    |              |
| ArteryTek    | AT32F423    |              |
| ArteryTek    | AT32F425    |              |
| ArteryTek    | AT32F435    |              |
| ArteryTek    | AT32F437    |              |
| ST           | STM32F072   | Need to redirect vectors in app or bootloader.   |
| ST           | STM32F103   |              |
| ST           | STM32F401   |              |
| ST           | STM32F407   |              |
| ST           | STM32F411   |              |
| ST           | STM32G431   |              |
| ST           | STM32L433   |              |
| WCH          | CH32V307    |              |
| WCH          | CH582M      |              |
| HDSC         | HC32F460    | Some problems with usb.And need to config icg if you want to use it in app.             |

## Todo List
- [ ] AT32WB415
- [ ] AT32F435/7(Multiport)
- [ ] HPM5300
- [ ] CH32V307 -> CH32V20X_30X
- [ ] BL618
- [ ] CH32F20X
- [ ] CH32V20X
- [ ] AG32VF103
- [ ] STM32H7
- [ ] MM32F327X
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