#!/usr/bin/env python

from __future__ import division

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
for i in range(1000):
    package.append(1) # used as bytes

# write the data out to the device
count = 0
now = datetime.datetime.now()
while count < 3000:
    wrote = dev.write(3, package, 1)

    count += 1
    if count % 1000 == 0:
        print count, ',', \
            1000*divideTimeDelta(datetime.timedelta(seconds=1), datetime.datetime.now()-now),\
            'packages/s, wrote', wrote, 'bytes'
        now = datetime.datetime.now()

