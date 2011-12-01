#!/usr/bin/env python

from __future__ import division

LEDS_PER_BOARD = 64
BOARDS = 2
PWM_STEPS = 15
NUM_COLORS = 3
NUM_ROWS = 1000

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

def pwm_data():
    ROW = BOARDS*LEDS_PER_BOARD*NUM_COLORS
    img = bytearray(ROW*NUM_ROWS);
    for i, pixel in enumerate(img):
        if i % 2:
            img[i] = 1

    board = 0 # boards -> 128 or 1

    buf = bytearray()
    for i in range(NUM_ROWS):
        num_leds = int(ROW/BOARDS/NUM_COLORS)
        index = i*ROW+board*num_leds*3
        relevant = img[index:index+num_leds*3]
        print ROW, index, num_leds

        # pwm calculation
        pwm = [0] * int(PWM_STEPS * num_leds * NUM_COLORS / 8)
        for j, pixel in enumerate(relevant):
            pixel = pixel >> 4
            for k in range(PWM_STEPS):
                led_id = num_leds / 3
                if k <= pixel:
                    address = (k*NUM_COLORS+(j % NUM_COLORS))*num_leds + int(j/NUM_COLORS)
                    #print 'a', address, PWM_STEPS, num_leds, j
                    pwm[int(address/8)] += 1 << address % 8
                else:
                    break
        pwm = img
        buf += pwm

        if i > 127:
            break

    print len(buf)

    return buf[:360*128]


# 46'080 bytes should at least be written, so that the buffer's full
# 360 bytes per row -> 360'000 for a full turn
package = ([255])*720*10
#package = ([0]+[0])*512

#package = pwm_data()
#package = ([0] + [0]*359)*2

def run(package=None):
    if package is None:
        package = ([1]+[0]*359)*129
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
run()
