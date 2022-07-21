# PiDGC Buildroot Repository

## Description

** This repo was cloned/modified from the repo at https://furkantokac.com/rpi3-fast-boot-less-than-2-seconds/ **

You will only need to use this if you want to modify/build your own Qt images. 

## Quick Start

### Build RPI3 fastboot distro + static Qt

1. make pidgc_defconfig
2. make -j8
3. ./build-rpi3-qt.sh

Output File | Path
--          | --
RPI3 Image  | output/images/sdcard.img
qmake       | output/qt/qt-everywhere-src-*/aaaout/bin/qmake