#!/usr/bin/env python
import led
import time
from cython import fastpwm

dev = led.init()

led.run(dev, [255] + [0]*63, timeout=2000)
data = str(bytearray(led.PWM_IMAGE_SIZE/2))
img = bytearray(led.PWM_IMAGE_SIZE/2)
for col in range(3):
    for j in range(16):
        for i, pixel in enumerate(img):
            img[i] = 0;

        for i, pixel in enumerate(img):
            if i % 48 == j*3:
                img[i] = 0x0C
                img[i+1] = 0x30;
                img[i+2] = 0xC3;

        print list(img[:48])
        fastpwm.pwm_2bit_c(str(img), 0, data)
        print list(bytearray(data[:48]))
        led.run(dev, data, timeout=5000)
        time.sleep(0.1)
