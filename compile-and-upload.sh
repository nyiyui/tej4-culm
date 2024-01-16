#!/usr/bin/env bash
# setup.sh compiles the Arduino code and uploads/flashes it to the Arduino.
# Ken Shibata
# Date: 2024-01-16
# Teacher: Mr. Wong
set -eux

arduino-cli compile --fqbn arduino:avr:uno
arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:uno
