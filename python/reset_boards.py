#!/usr/bin/env python
from serial_board import syncboard

sync = syncboard.Syncboard('/dev/ttyUSB0')

sync.reset_led_boards()
