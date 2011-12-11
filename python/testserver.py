#!/usr/bin/env python
import httplib
import urllib

con = httplib.HTTPConnection('localhost:8080')

test = bytearray(10)
test[1] = 1
test[2] = 2
test[3] = 3
params = {'@data': 'asdf'}
con.request("POST", "/image", test)
print con.getresponse().status, "\n"
