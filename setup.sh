#!/usr/bin/env bash
# setup.sh sets up the developer environment to compile and flash the Arduino.
# Ken Shibata
# Date: 2024-01-16
# Teacher: Mr. Wong
set -eux

arduino-cli lib install 'Adafruit NeoPixel'
