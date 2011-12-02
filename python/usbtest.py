#!/usr/bin/env python

from __future__ import division

LEDS_PER_BOARD = 64
BOARDS = 2
PWM_STEPS = 15
NUM_COLORS = 3
NUM_ROWS = 1000
ROW = BOARDS*LEDS_PER_BOARD*NUM_COLORS
NUM_LEDS = int(ROW/BOARDS/NUM_COLORS)

import os
os.environ['PYUSB_LOG_FILENAME'] = 'test.log'
#os.environ['PYUSB_DEBUG_LEVEL'] = 'debug'

import usb.core
import usb.util
import datetime

def divideTimeDelta(td1, td2):
    us1 = td1.microseconds + 1000000 * (td1.seconds + 86400 * td1.days)
    us2 = td2.microseconds + 1000000 * (td2.seconds + 86400 * td2.days)
    return us1 / us2


# main script!
dev = None
def init():
    global dev
    dev = usb.core.find(idVendor=0xffff, idProduct=0x5740)

    if dev is None:
        raise ValueError('Device not found')

    # set the active configuration. With no arguments, the first
    # configuration will be the active one
    dev.set_configuration()

"""
print 'configs:'
for cfg in dev:
    print cfg.bConfigurationValue
    for intf in cfg:
        print '\t' + str(intf.bInterfaceNumber) + ',' + str(intf.bAlternateSetting)
        for ep in intf:
            print '\t\t' + str(ep.bEndpointAddress)
"""

def pwm_data(img):
    board = 0 # boards -> 128 or 1

    buf = bytearray()
    for i in range(NUM_ROWS):
        index = int(i*ROW+ROW/2*board)
        relevant = img[index:index+int(ROW/2)]
        #print ROW, index, NUM_LEDS

        # pwm calculation
        pwm = bytearray(int(PWM_STEPS * NUM_LEDS * NUM_COLORS / 8))
        for j, pixel in enumerate(relevant):
            pixel = pixel >> 4
            for k in range(PWM_STEPS):
                led_id = NUM_LEDS / 3
                if k <= pixel:
                    address = (k*NUM_COLORS+(j % NUM_COLORS))*NUM_LEDS + int(j/NUM_COLORS)
                    #print 'a', address, PWM_STEPS, NUM_LEDS, j
                    pwm[int(address/8)] += 1 << (address % 8)
                else:
                    break

        print list(pwm)
        buf += pwm

        if i > 127:
            break

    print len(buf)

    return buf[:360*128]


# 46'080 bytes should at least be written, so that the buffer's full
# 360 bytes per row -> 360'000 for a full turn

def run(package=None):
    if package is None:
        package = [0]*360*128
        #package = (([1]+[0]*7)*15*3)*128
        #package = ([255]*360)*128
        #package = ([1]*3+[0]*46077)
    # write the data out to the device
    count = 0
    now = datetime.datetime.now()
    report_all = 1

    #while count < 10:
    wrote = dev.write(3, package, 1)

    count += 1
    if count % report_all == 0:
        print count, ',', \
            report_all*divideTimeDelta(datetime.timedelta(seconds=1),\
            datetime.datetime.now()-now),\
            'packages/s, wrote', wrote, 'bytes'
        now = datetime.datetime.now()


    #usb.util.release_interface(dev, 1)
    #usb.util.dispose_resources(dev)

init()

img = bytearray(ROW*NUM_ROWS);
for i, pixel in enumerate(img):
    if i % 3 == 0:
        img[i] = i % NUM_LEDS

run(pwm_data(img))
"""
import time
while(1):
    # all LEDs
    run(([0]*120+[255]*120+[0]*120)*128) # red
    time.sleep(1)
    run(([0]*240+[255]*120)*128) # green
    time.sleep(1)
    run(([255]*120+[0]*240)*128) # blue
    time.sleep(1)
    run(([255]*360)*128) # white
    time.sleep(1)

    run(([255]*120+[0]*120+[255]*120)*128) # cyan
    time.sleep(1)
    run(([255]*240+[0]*120)*128) # magenta
    time.sleep(1)
    run(([0]*120+[255]*240)*128) # yellow
    time.sleep(1)

    # 1 LED
    # run(([1]+[0]*7)*15+[0]*240)
    # time.sleep(1)
    # run([0]*120+([1]+[0]*7)*15+[0]*120)
    # time.sleep(1)
    # run([0]*240+([1]+[0]*7)*15)
    # time.sleep(1)
    # run(([1]+[0]*7)*3*15)
    # time.sleep(1)
"""
