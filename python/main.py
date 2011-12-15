#!/usr/bin/env python
import led
import time
import image
from server import TcpServer
import SocketServer
from serial_board import syncboard
import serial

def handle_serial_event(key, value):
    """ callback function for Syncboard """
    print key, value

    #if led.image_current > led.image_pwm_current
    if key == 'rota':
        t = int(value, 16)
        print t


class RotatingLed(object):

    calc_thread = None
    board_threads = []

    def start(self):
        """ starts the whole program to run an led animation """
        # initialize buffers
        image.initialize_buffers()

        # start threads:
        self.calc_thread = image.PwmCalculation(debug=True)
        self.calc_thread.start()

        boards = led.init(find_all=True)
        self.board_threads = []
        for b in boards:
           print "found board", b
           t = image.Board(b, data)
           t.start()
           self.board_threads.append(t)

        # start synchronisation
        try:
            self.sync = syncboard.Syncboard('/dev/ttyUSB0', handle_serial_event)
            self.sync.setVar('dip', '5') # remove later, just a debugging option
            self.sync.startReading()
        except serial.SerialException:
            print 'Sync board not found!'


        # start server
        HOST, PORT = "localhost", 28900
        self.server = SocketServer.TCPServer((HOST, PORT), TcpServer.MyTCPHandler)
        self.server.serve_forever()

    def stop(self):
        """ stops the whole program to run an led animation """
        self.server.shutdown()

        self.calc_thread.stop()
        self.calc_thread.join()

        for b in self.boards_threads:
            b.stop()
            b.join()

        self.sync.stopReading()
        self.sync.close()

if __name__ == "__main__":
    b = RotatingLed()
    try:
        b.start()
    except Exception:
        import traceback
        traceback.print_exc()
        b.stop() 
