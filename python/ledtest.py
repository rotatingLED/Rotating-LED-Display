#!/usr/bin/env python
import led
import time

dev = led.init()

led.run(dev, [255] + [0]*63, timeout=2000)
def pwm_1led(index):
    i = led.LED_ARR[index]
    shift = i % 8;
    number = [1 << shift]
    col = [0]*(i/8)+number+[0]*((63-i)/8)
    return col
    
for col_nr in range(3):
    for i in range(64):
        col = pwm_1led(i)

        if col_nr == 0:
            data = (2*col + 32*[0])*led.NUM_ROWS
        elif col_nr == 1:
            data = (16*[0]+2*col + 16*[0])*led.NUM_ROWS
        else:
            data = (32*[0] + 2*col)*led.NUM_ROWS

        led.run(dev, data, timeout=5000)
