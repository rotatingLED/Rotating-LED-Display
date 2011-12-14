import led
import time
import image
import multiboards
import server.TcpServer
import SocketServer
from serial import syncboard

def handle_serial_event(key, value):
    """ callback function for Syncboard """
    print key, value

    if key == 'rota':
        t = int(value, 16)
        print t


class RotatingLedBoard(object):
    calc_thread = None
    board_threads = []
    def start(self):
        """ starts the whole program to run an led animation """
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

        # initialize buffers
        image.initialize_buffers()

        # start server
        HOST, PORT = "localhost", 28900
        server = SocketServer.TCPServer((HOST, PORT), MyTCPHandler)
        server.serve_forever()

        # start synchronisation
        self.sync = syncboard.Syncboard('/dev/ttyUSB0', handle_serial_event)
        self.sync.setVar('dip', '5') # remove later, just a debugging option
        self.sync.startReading()

    def stop(self):
        """ stops the whole program to run an led animation """
        server.shutdown()

        self.calc_thread.stop()
        self.calc_thread.join()

        for b in self.boards_threads:
            b.stop()
            b.join()

        self.sync.stopReading()
        self.sync.close()

if __name__ == "__main__":
    b = RotatingLedBoard()
    while 1:
        try:
            b.start()
            while 1:
                time.sleep(1)
        except Exception, e:
            print e
            b.stop() 
