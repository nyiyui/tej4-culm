#!/usr/bin/env bash
set -eux

arduino-cli compile --fqbn arduino:avr:uno
arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:uno
