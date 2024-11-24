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
| Manufacturer | MCU       | Note                                                                        | sdk |
| :----------- | :-------- | :-------------------------------------------------------------------------- | :-------------------------------------------------------------------------- |
| ArteryTek    | AT32F402  |                                                                             | [AT32F402_405_sdk](https://github.com/ArteryTek/AT32F402_405_Firmware_Library.git) |
| ArteryTek    | AT32F403A |                                                                             | [AT32F403A_407_sdk](https://github.com/ArteryTek/AT32F403A_407_Firmware_Library.git)
| ArteryTek    | AT32F405  |                                                                             | [AT32F402_405_sdk](https://github.com/ArteryTek/AT32F402_405_Firmware_Library.git)
| ArteryTek    | AT32F407  |                                                                             | [AT32F403A_407_sdk](https://github.com/ArteryTek/AT32F403A_407_Firmware_Library.git)
| ArteryTek    | AT32F413  |                                                                             | [AT32F413_sdk](https://github.com/ArteryTek/AT32F413_Firmware_Library.git)
| ArteryTek    | AT32F415  |                                                                             | [AT32F415_sdk](https://github.com/ArteryTek/AT32F415_Firmware_Library.git)
| ArteryTek    | AT32F423  |                                                                             | [AT32F423_sdk](https://github.com/ArteryTek/AT32F423_Firmware_Library.git)
| ArteryTek    | AT32F425  |                                                                             | [AT32F425_sdk](https://github.com/ArteryTek/AT32F425_Firmware_Library.git)
| ArteryTek    | AT32F435  |                                                                             | [AT32F435_437_sdk](https://github.com/ArteryTek/AT32F435_437_Firmware_Library.git)
| ArteryTek    | AT32F437  |                                                                             | [AT32F435_437_sdk](https://github.com/ArteryTek/AT32F435_437_Firmware_Library.git)
| ArteryTek    | AT32WB415 |                                                                             | [AT32WB415_sdk](https://github.com/ArteryTek/AT32WB415_Firmware_Library.git)

## Todo List
- [ ] Arterytek
- [ ] HPM
- [ ] WCH
- [ ] ST

# How to build
- Download the sdk of mcu.
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