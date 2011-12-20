#!/usr/bin/env python

import led

dev = led.init()

print dev.write(3, [0]*8, 1)
print dev.write(1, [255]+[0]*7, 1)
