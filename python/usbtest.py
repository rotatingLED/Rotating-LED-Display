#!/usr/bin/env python

import led

# 46'080 bytes should at least be written, so that the buffer's full
# 360 bytes per row -> 360'000 for a full turn 1000 pixels per second

led.init()

import time
while(1):
    # all LEDs
    led.run(([0]*120+[255]*120+[0]*120)*128) # red
    time.sleep(1)
    led.run(([0]*240+[255]*120)*128) # green
    time.sleep(1)
    led.run(([255]*120+[0]*240)*128) # blue
    time.sleep(1)
    led.run(([255]*360)*128) # white
    time.sleep(1)

    led.run(([255]*120+[0]*120+[255]*120)*128) # cyan
    time.sleep(1)
    led.run(([255]*240+[0]*120)*128) # magenta
    time.sleep(1)
    led.run(([0]*120+[255]*240)*128) # yellow
    time.sleep(1)

    # 1 LED
    # led.run(([1]+[0]*7)*15+[0]*240)
    # time.sleep(1)
    # led.run([0]*120+([1]+[0]*7)*15+[0]*120)
    # time.sleep(1)
    # led.run([0]*240+([1]+[0]*7)*15)
    # time.sleep(1)
    # led.run(([1]+[0]*7)*3*15)
    # time.sleep(1)
