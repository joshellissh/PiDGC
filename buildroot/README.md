# ftDev Buildroot Repository

## Description

This branch is specialized for Raspberry Pi 3 for now.

Base Buildroot Version  : 2019.02.5

For details, you can read my blogpost: https://furkantokac.com/rpi3-fast-boot-less-than-2-seconds/


## Scripts

Name             | Function
--               | --
build-rpi3-qt.sh | Builds static Qt. Run this after build RPI3 image.


## Quick Start

### Build RPI3 fastboot distro + static Qt

1. make ftdev_rpi3_fastboot_defconfig
2. make -j8
3. ./build-rpi3-qt.sh

Output File | Path
--          | --
RPI3 Image  | output/images/sdcard.img
qmake       | output/qt/qt-everywhere-src-*/aaaout/bin/qmake


make pidgc_defconfig
make -j8
1280x480
make linux-menuconfig
- Device Drivers
  - Input device support
    - <*> Mouse interface
    - [*] USB support
      - [*] Keyboards
        - Select all as *
      - [*] Mice
        - Select all as *
      - [*] Touchscreens
  - HID support
    - Special HID drivers
      - <*> A4 Tech mice
      - <*> Apple {i,Power,Mac}Books
      - <*> ASUS
      - <*> Logitech devices
        - All subdivices
      - <*> Apple Magic Mouse/Trackpad multi-touch support
    - USB HID support
      - Select all as *
  - USB support
    - [*]   USB announce new devices
    - Set all as * under `*** USB Host Controller Drivers ***`