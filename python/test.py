#!/usr/bin/env python
import led
import image
from cython import fastpwm

boards = led.init(find_all=True)

buf = str(bytearray(led.PWM_IMAGE_SIZE/2))
fastpwm.pwm_2bit_c(str(bytearray([0x0C,0x30,0xC3]*(64*344/4))), 0, buf)

red   = str(bytearray(([255]*16+[0]*16+[0]*16)*344))
green = str(bytearray(([0]*16+[255]*16+[0]*16)*344))
blue  = str(bytearray(([0]*16+[0]*16+[255]*16)*344))
blue2  = str(bytearray(([0]*16+[0]*16+[255]*16)*344))

fastpwm.pwm_2bit_c(str(bytearray([0x0C,0x70,0xC7]*(64*344/4))), 0, blue)

images = [
    buf, buf, buf, buf, buf, buf, buf, buf, 
    [0]*(led.PWM_IMAGE_SIZE/2),
    [0]*(led.PWM_IMAGE_SIZE/2),
    [0]*(led.PWM_IMAGE_SIZE/2),
    [0]*(led.PWM_IMAGE_SIZE/2),
    [0]*(led.PWM_IMAGE_SIZE/2),
    [0]*(led.PWM_IMAGE_SIZE/2),
    [0]*(led.PWM_IMAGE_SIZE/2),
    [0]*(led.PWM_IMAGE_SIZE/2),


]

images = [red, green, blue, blue2]

for b in boards:
    print b
    temp = 11862005 / led.NUM_ROWS
    led.run(b, [2]+[temp & 255]+[(temp >> 8) & 255]+
                     [(temp >> 16) & 255] + [(temp >> 24) & 255],
            endpoint=1);
    led.run(b, [255] + [0]*63, timeout=2000)
    for i in range(100):
        for img in images:
            pass
            led.run(b, img, timeout=5000)
