## Overview

Arduino sketch to dump the base64-encoded contents of an SD Card to the serial
port.

### tl;dr

    $ cd <project_dir>
    $ ARDUINO_DIR=/usr/share/arduino/ ARDMK_DIR=/usr/share/arduino/ make upload
    $ sudo stty -F /dev/ttyACM0 ispeed 115200
    $ (stty raw; cat >/tmp/dump_sd.log) </dev/ttyACM0


## Introduction

This Arduino sketch dumps the contents of an attached SD card to the serial
port. The sketch queries the card for its capacity, sequentially reads each
512B block, and, for each successfully-read block, encodes the block as two
base64-encoded strings and prints them to the serial port.

If a block read reports an error, the sketch prints an error message including
the block number.


## Prerequisites

* Arduino Core & Arduino Makefile. On Ubuntu,

    sudo apt-get install arduino-core arduino-mk

* Arduino Uno attached to `/dev/ttyACM0`
* [SD card shield](http://www.microcenter.com/product/476328/SD_Card_Logging_Shield_-_2_Pack)


## Hardware setup

Wiring for the [shield I used](https://forum.vellemanprojects.eu/t/vma304-no-documentation/28218/2):

* +5v
* Ground
* CS → Pin 10
* MOSI → Pin 11
* MISO → Pin 12
* SCK → Pin 13

You may need to adjust the wiring and/or source to match your hardware.


## Compilation

This sketch is built using the [Arduino Makefile](https://github.com/sudar/Arduino-Makefile)
command line toolchain. To build the sketch, open a terminal and

    $ cd <project_dir>
    $ ARDUINO_DIR=/usr/share/arduino/ ARDMK_DIR=/usr/share/arduino/ make

If you have an Arduino Uno attached to `/dev/ttyACM0`, you can uplaod the
sketch with

    $ ARDUINO_DIR=/usr/share/arduino/ ARDMK_DIR=/usr/share/arduino/ make upload


## Getting the data

For setup and testing, I used `minicom`

    $ minicom -D /dev/ttyACM0

For an actual SD card dump/recovery attempt, I would use the terminal directly
rather than trying to log the data from `minicom`. You need to set the TTY
parameters to match the Arduino’s serial port configuration

    $ sudo stty -F /dev/ttyACM0 ispeed 115200

With that done, you can simply `cat` the data to a convenient file

    $ (stty raw; cat >/tmp/dump_sd.log) </dev/ttyACM0


## Converting the data to a binary image

Use a text editor to get rid of the header & footer information that the
sketch includes, then something like the following Python will convert the
data (assuming no error blocks) to the binary image from the card:

    #!/usr/bin/env python
    #
    # python dump_to_binary.py <infile> <outfile>

    import sys


    with open(sys.argv[1], 'rb') as f:
        data = f.read()

    decoded = ''.join(line.decode('base64') for line in data.split('\n'))

    with open(sys.argv[2], 'wb') as f:
        f.write(data)

Holler at me if you'd like for me to turn this snippet into a proper script. :)


## References and Resources

* [How to Use MMC/SDC](http://elm-chan.org/docs/mmc/mmc_e.html), with detailed
  specifics about the SPI interface to SD cards.
* The [Arduino SD Library Reference](https://www.arduino.cc/en/Reference/SD)
* Another [SD Card File/Image Recovery Tool](http://tech.tiefpunkt.com/2013/08/sd-card-recovery-using-an-arduino/)
* SO posts detailing problems and approaches to SD card SPI init
  * [What is the correct command sequence…](https://electronics.stackexchange.com/q/77417)
  * [Initialization of a microSD card using an SPI interface](https://stackoverflow.com/q/955587)
  * [Initializing SD card in SPI issues](https://stackoverflow.com/q/2365897)
