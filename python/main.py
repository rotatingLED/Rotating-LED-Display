#!/usr/bin/env python
import led
import time
import image
from server import TcpServer
import SocketServer
from serial_board import syncboard
import serial



class RotatingLed(object):

    calc_thread = None
    board_threads = []

    def __init__(self, init_server=True):
        self.init_server = init_server

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
           t = image.Board(b)
           t.start()
           self.board_threads.append(t)

        # start synchronisation
        try:
            self.sync = syncboard.Syncboard('/dev/ttyUSB0', self.handle_serial_event)
            #self.sync.setVar('dip', '180') # remove later, just a debugging option
            # adc threshhold
            self.sync.setVar('adc', '200')
            self.sync.startReading()
        except serial.SerialException:
            print 'Sync board not found!'


        if self.init_server:
            # start server
            HOST, PORT = "localhost", 28900
            self.server = SocketServer.TCPServer((HOST, PORT), TcpServer.MyTCPHandler)
            self.server.serve_forever()

            # start server
            HOST, PORT = "localhost", 28900
            #self.server = SocketServer.TCPServer((HOST, PORT), TcpServer.MyTCPHandler)
            #self.server.serve_forever()
        else:
            # just show a debug picture
            fh = open('testbild.bin', 'rb')
            image.image_buffer[1] = fh.read()
            print 'testbild_size', len(image.image_buffer[1])
            image.image_current += 1
            while(1):
                time.sleep(1)

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

    def handle_serial_event(self, key, value):
        """ callback function for Syncboard """
        #print key, value

        #if led.image_current > led.image_pwm_current
        if key == 'rota':
            t = int(value, 16)
            #print t
            for b in self.board_threads:
                b.send_rotation_time(t)

if __name__ == "__main__":
    main = RotatingLed(init_server = False)
    try:
        main.start()
    except Exception:
        import traceback
        traceback.print_exc()
        #main.stop() 
