#!/usr/bin/env python
import led
import time

dev = led.init()

img = bytearray(led.ROW*led.NUM_ROWS);
for col in range(3):
    for j in range(64):
        for i, pixel in enumerate(img):
            # img[i] = 0
            # if i % 192 == j*3+col:
                img[i] = 255

        data = led.pwm_data(img)
        if len(data) != 46080:
            raise Exception('length is too short')
        led.run(dev, data)
        time.sleep(0.1)
