#!/usr/bin/env python
import led

led.init()

img = bytearray(led.ROW*led.NUM_ROWS);
for i, pixel in enumerate(img):
    if i % 3 == 0:
        img[i] = (i % led.LEDS_PER_BOARD)*4

data = led.pwm_data(img)
print len(data)

led.run(data)
