# Cherryuf2
This repo is cross-platform UF2 Bootloader projects for MCUs based on Cherryusb.It is a simple bootloader that forks from [tinyuf2](https://github.com/adafruit/tinyuf2) and migrates the USB stack to [CherryUSB](https://github.com/sakumisu/CherryUSB).
It only supports DFU with MSC.
```
.
├── lib                   # Sources from 3rd party such as cherryusb, uf2 ...
├── sdk                   # Sources from 3rd party such as mcu drivers
├── ports                 # Port/family specific sources
│   ├── ArtertTek         # MCU manufacturer
│       └── at32f413      # MCU Series
|           └── boards/   # Board specific sources
│           └── Makefile  # Makefile for this port         
├── src                   # Cross-platform bootloader sources files
```

# Support Devices
| Manufacturer | MCU         |
| :----------  | :---------- | 
| ArteryTek    | AT32F413    |

# How to build
```
$ cd ports/ArtertTek/at32f413
$ make BOARD=at32f413cbt7
```
