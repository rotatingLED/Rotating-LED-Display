#!/usr/bin/env python

#import led
import time
import led
from cython import fastpwm

img = bytearray(led.ROW*led.NUM_ROWS);
for i, pixel in enumerate(img):
    # img[i] = 0
    # if i % 192 == j*3+col:
        img[i] = 255

for i in range(100):
    pwm_data = str(bytearray(led.PWM_IMAGE_SIZE))
    data = fastpwm.pwm_4bit_c(str(img), 0, pwm_data)

for i in range(10):
    #data = fastpwm.pwm_data_4bit(str(img))
    pass

#data = led.pwm_data(str(img))

#print list(data[:100])
#print list(pwm_data[:100])

print "done\n"
