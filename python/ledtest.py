#!/usr/bin/env python
import led
import time

led.init()

def pwm_1led(index):
    i = led.LED_ARR[index]
    shift = i % 8;
    number = [1 << shift]
    col = [0]*(i/8)+number+[0]*((63-i)/8)
    return col
    
img = bytearray(led.ROW*led.NUM_ROWS);
leds = [0]*360*128
for col_nr in range(3):
    for i in range(64):
        col = pwm_1led(i)

        if col_nr == 0:
            data = (15*col + 240*[0])*128
        elif col_nr == 1:
            data = (120*[0]+15*col + 120*[0])*128
        else:
            data = (240*[0] + 15*col)*128
        led.run(data)
        time.sleep(0.1)

