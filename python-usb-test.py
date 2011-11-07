#!/usr/bin/env python

from __future__ import division

import os
os.environ['PYUSB_LOG_FILENAME'] = 'test.log'
os.environ['PYUSB_DEBUG_LEVEL'] = 'debug'

import usb.core
import usb.util
import datetime

def divideTimeDelta(td1, td2):
    us1 = td1.microseconds + 1000000 * (td1.seconds + 86400 * td1.days)
    us2 = td2.microseconds + 1000000 * (td2.seconds + 86400 * td2.days)
    return us1 / us2


# main script!
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

package = []
for i in range(100*1023):
    if i % 2 == 1:
        package.append(1) # used as bytes
    else:
        package.append(2) # used as bytes


# write the data out to the device
count = 0
now = datetime.datetime.now()
report_all = 10
while 1:
    wrote = dev.write(3, package, 1)

    count += 1
    if count % report_all == 0:
        print count, ',', \
            report_all*divideTimeDelta(datetime.timedelta(seconds=1),\
            datetime.datetime.now()-now),\
            'packages/s, wrote', wrote, 'bytes'
        now = datetime.datetime.now()

