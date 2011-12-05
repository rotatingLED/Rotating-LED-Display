#!/usr/bin/env python
import led
import time

led.init()

for col in range(3):
    img = bytearray(led.ROW*led.NUM_ROWS);
    counter = 0;
    for i, pixel in enumerate(img):
        if i % 3 == col:
            counter += 1
            if counter >= 64:
                counter = 0
            img[i] = min(counter*4, 255)

    data = led.pwm_data(img)

    led.run(data)
    time.sleep(2)
