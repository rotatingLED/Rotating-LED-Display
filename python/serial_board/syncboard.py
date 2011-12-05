#!/usr/bin/env python

import time
import serial
import threading
import led
import fcntl

class Syncboard(object):
    def __init__(self, port, handle_function=None):
        self._reader_alive = False
        self.handle_function = handle_function
        # configure the serial connections (the parameters differs on the device you are connecting to)
        self.ser = serial.Serial(
            port = port,
            baudrate = 57600,
            parity = serial.PARITY_NONE,
            stopbits = serial.STOPBITS_TWO,
            bytesize = serial.EIGHTBITS
        )

        self.ser.open()

    def startReading(self):
        """Start reader thread"""
        self._reader_alive = True
        self.receiver_thread = threading.Thread(target=self.reader)
        self.receiver_thread.setDaemon(True)
        self.receiver_thread.start()

    def stopReading(self):
        """Stop reader thread only, wait for clean exit of thread"""
        self._reader_alive = False
        self.receiver_thread.join()

    def reader(self):
        """reader thread for input"""
        try:
            s = ''
            while self._reader_alive:
                if self.ser.inWaiting() > 0:
                    # self.ser.read(1) is blocking until a char arrives, maybe never if the board sends nothing
                    data = self.ser.read(1)
                else:
                    time.sleep(0.05) # 50ms
                    continue

                if ord(data) == 13 or ord(data) == 10:
                    try:
                        index = s.index('=')
                        if self.handle_function is not None:
                            self.handle_function(s[:index], s[index+1:])
                    except Exception, e:
                        pass
                        #print 'error:', s, e
                    s = ''
                else:
                    s += data;

        except serial.SerialException, e:
            self._reader_alive = False
            raise e

    def close(self):
        self.ser.close();

    def readLineTimeout(self):
        """reads a line with a short timeout"""
        line = ''
        data = ' '
        count = 0
        while ord(data) != 13 and ord(data) != 10:
            if self.ser.inWaiting() > 0:
                data = self.ser.read(1)
                line += data
                count = 0
            else:
                time.sleep(0.05) # 50ms
                count += 1

                # more than 5s?
                if count > 100:
                    raise Exception("Timeout occured, didn't get line")

        return line.strip()

    def setVar(self, name, value):
        """sets a configuration var on the syncboard"""
        readerAlive = False
        if self._reader_alive:
            readerAlive = True
            sync.stopReading()

        # configure command
        self.ser.write('c')

        # empty queue until '>'
        data = None
        count = 0
        while data != '>':
            if self.ser.inWaiting() > 0:
                data = self.ser.read(1)
            else:
                time.sleep(0.05) # 50ms
                count += 1

                # more than 5s?
                if count > 100:
                    raise Exception('Timeout occured, configuration not possible')

        self.ser.write('set ' + name + '=' + value + '\n')
        self.ser.flush()

        line = self.readLineTimeout()
        if line != '$ set':
            raise Exception('Unexpected answer: "%s"' % line)

        line = self.readLineTimeout()
        if line != ('$ ' + name + '=' + value):
            raise Exception('Unexpected answer: "%s"' % line)

        self.ser.write('exit\n')        
        line = self.readLineTimeout()
        if line != '>$ exit':
            raise Exception('Unexpected answer: "%s"' % line)

        if readerAlive:
            sync.startReading()

    def reset_led_boards(self):
        self.setVar('reset', '1')
        time.sleep(0.001)
        self.setVar('reset', '0')

        # reset not only the boards, but also the usb connection
        boards = led.get_boards(find_all=True)
        print 'numboards:', boards
        for b in boards:
            print "Reset Board on Bus %i, Device %i" % (b.bus, b.address)

            fd = open('/dev/bus/usb/%03i/%03i' % (b.bus, b.address), 'w')
            # defined somewhere in the c libraries of the system
            USBDEVFS_RESET = 21780
            # have a try here, because sometimes it seems to work,
            # but throws an exception.
            try:
                fcntl.ioctl(fd, USBDEVFS_RESET, 0);
            except:
                pass
        

    def flash_led_boards(self):
        self.setVar('boot0', '1')
        self.setVar('reset', '1')
        time.sleep(0.001)
        self.setVar('reset', '0')
        time.sleep(0.001)
        self.setVar('boot0', '0')

def handle_serial_event(key, value):
    """ callback function for Syncboard """
    print key, value

    if key == 'rota':
        t = int(value, 16)
        print t

if __name__ == '__main__':
    print 'configuration'
    sync = Syncboard('/dev/ttyUSB0', handle_serial_event)
    sync.setVar('dip', '11')
    print 'sync.startReading()'
    sync.startReading()
    time.sleep(3);

    print 'sync.stopReading()'
    sync.stopReading()
    print 'sync.close()'
    sync.close()

