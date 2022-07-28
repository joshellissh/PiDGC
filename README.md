# PiDGC v2
Raspberry Pi based Digital Gauge Cluster by Josh Ellis / josh@ellis.sh

**Please Note: This library is designed for the Raspberry Pi 3B, a custom built PCB, and a 1280x480 display. It
will look weird on other displays and won't function without those components.**

### Update Information
This repository is a total rewrite of the PiDGC codebase. The kotlin hardware interface has 
been removed, and all code is now in either the microcontroller (Teensy 4.1) or the Qt 
application running on the RPi. These changes were made to simplify the system and significantly 
shorten the boot time.

### Acknowledgements
In order to reduce the boot time significantly, this version of the gauge cluster uses [buildroot](https://buildroot.org/).
A fork of the [configuration](https://github.com/furkantokac/buildroot) made by [Furkan Tokaç](https://furkantokac.com/rpi3-fast-boot-less-than-2-seconds/) 
is the basis of this build, so many thanks for the reduced boot time go to him! Please go and check out his work, 
it's great.

### Software Steps
Since this is a buildroot build and no binaries are provided, you must build the image for the Raspberry Pi 3 yourself.
This isn't a complicated process, but does require either a Linux system or WSL2.

**These are high level steps only. If you have questions about how to do any step, please Google it.**

1. `cd` into the `buildroot` directory
2. `make pidgc_defconfig`
3. `make -j8`
4. `./build-rpi3-qt.sh`
5. Setup Qt Creator to use the qmake found in `buildroot/output/qt/qt-everywhere-src-*/aaaout/bin/qmake` and compile for the Raspberyr Pi 3.
6. Open `source/display` in Qt Creator and compile for RPi3.
7. Run `make -j8` again. This should pull in the `rpi3-display/display` binary and create the `sdcard.img` in `buildroot/output/images/sdcard.img`.
8. Burn the image file to a micro SD card.
9. All done!

### Custom PCB
The PCB was designed in EAGLE, and is located in `files/PiDGCv2-pcb`