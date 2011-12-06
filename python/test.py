#!/usr/bin/env python
import led
import image
from cython import fastpwm

boards = led.init(find_all=True)

buf = str(bytearray(led.PWM_IMAGE_SIZE/2))
fastpwm.pwm_4bit_c(str(bytearray([0,15,240]*64*1000)), 0, buf)

images = [
    [255]*(led.PWM_IMAGE_SIZE/2),
    #[0]*(led.PWM_IMAGE_SIZE/2),
    buf,
]

for b in boards:
    print b
    led.run(b, [255] + [0]*63, timeout=2000)
    for i in range(10):
        for img in images:
            pass
            led.run(b, img, timeout=5000)
