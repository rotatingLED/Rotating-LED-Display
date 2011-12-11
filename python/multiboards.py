#!/usr/bin/env python

import led
import time
import threading

led.init()

class Board(threading.Thread):
    def __init__(self, dev, data):
        threading.Thread.__init__(self)
        self.data = data
        self.dev = dev

    def run(self):
        i = 0
        while i < 10:
            led.run(self.dev, data)


def pwm_1led(index):
    i = led.LED_ARR[index]
    shift = i % 8;
    number = [1 << shift]
    col = [0]*(i/8)+number+[0]*((63-i)/8)
    return col
    


if __name__ == '__main__':
    img = bytearray(led.ROW*led.NUM_ROWS);
    leds = [0]*360*128
    for col_nr in range(3):
        for i in range(64):
            col = pwm_1led(i)

            if col_nr == 0:
                data = (15*col + 240*[0])*128
            elif col_nr == 1:
                data = (120*[0]+15*col + 120*[0])*128
            else:
                data = (240*[0] + 15*col)*128
            #led.run(data)
            time.sleep(0.1)
            break
        break

    boards = led.init(find_all=True)
    threads = []
    for b in boards:
        print "found board", b
        t = Board(b, data)
        t.start()
        threads.append(t)

    for t in threads:
        t.join()
