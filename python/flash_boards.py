#!/usr/bin/env python
from serial_board import syncboard

sync = syncboard.Syncboard('/dev/ttyUSB0')

sync.flash_led_boards()
