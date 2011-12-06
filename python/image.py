"""image - responsible for the streaming of images to the LED boards

The module owns the classes:
- PwmCalculation
- Board

and owns the image buffers and counters.
"""

import led
from cython import fastpwm
import time
import threading
from usb import USBError

# one image ~ 200K
IMAGE_BUFFER_LENGTH = 64

image_current = None
image_pwm_current = None
image_buffer = None
image_pwm_buffer = None
image_buffer = None

def initialize_buffers():
    global image_current, image_pwm_current, image_buffer, image_pwm_buffer
    image_current = 0
    image_pwm_current = -1
    image_buffer     = [None]*IMAGE_BUFFER_LENGTH
    image_pwm_buffer = [[str(bytearray(led.PWM_IMAGE_SIZE/led.BOARDS))]*IMAGE_BUFFER_LENGTH,
                        [str(bytearray(led.PWM_IMAGE_SIZE/led.BOARDS))]*IMAGE_BUFFER_LENGTH]
    image_buffer[0]  = str(bytearray([255]*led.IMAGE_SIZE))


class PwmCalculation(threading.Thread):
    def __init__(self, debug=False):
        threading.Thread.__init__(self)
        self.setDaemon(True)
        self.debug = debug
        self.running = True

    def run(self):
        """ run is called by the thread and calculates the pwm images """
        global image_current, image_pwm_current, image_buffer, image_pwm_buffer
        while self.running:
            # check if the calculation is too slow
            if (image_current - IMAGE_BUFFER_LENGTH) >= image_pwm_current:
                # maybe raise an exception?
                print 'Buffer underrun!'
            if image_pwm_current < image_current:
                index = (image_pwm_current+1) % IMAGE_BUFFER_LENGTH
                fastpwm.pwm_2bit_c(image_buffer[index], 0, image_pwm_buffer[0][index])
                fastpwm.pwm_2bit_c(image_buffer[index], 1, image_pwm_buffer[1][index])
                image_pwm_current += 1
            else:
                time.sleep(0.01)

    def stop(self):
        """ stop the thread and make it possible to 'join' threads """
        self.running = False


class Board(threading.Thread):

    def __init__(self, dev, board=0):
        threading.Thread.__init__(self)
        self.setDaemon(True)
        self.dev = dev
        self.synchronize = True
        self.running = True
        self.image_pwm_current = image_pwm_current
        self.board = board

    def run(self):
        """ run is called by the thread and is an endless loop to send data 
            to the led boards
        """
        # realtime stuff - the shit!
        while self.running:
            try:
                if self.synchronize:
                    #self.send_control_data(1, [0]*7)
                    #time.sleep(0.01)
                    led.run(self.dev, [255] + [0]*63)
                    self.synchronize = False
                else:
                    self._send_current_picture()
            except USBError, e:
                if e.errno == 110: # usb operation timeout
                    print 'USBError: [Errno 110] Operation timed out'
                    self.synchronize = True
                    time.sleep(0.1)
                else:
                    raise e

    def _send_current_picture(self):
        """ sends the current picture and no more """
        # 2 boards, so just take one
        if self.image_pwm_current >= 0:
            #for byte in image_pwm_buffer[0][0][:100]:
                #print "%02x" % ord(byte)
            led.run(self.dev, image_pwm_buffer[self.board][self.image_pwm_current],
                        timeout=5000)
        if image_pwm_current > self.image_pwm_current: 
            self.image_pwm_current += 1

    def stop(self):
        """ stop the thread and make it possible to 'join' threads """
        self.running = False

    def send_control_data(self, command, data):
        """ send data to the control endpoint, which is the endpoint 4 """
        led.run(self.dev, [command] + data, endpoint=1)

    def send_rotation_time(self, rotation_time):
        """ sends the rotation time, but not for the whole thing,
            just for each pixel """
        temp = rotation_time/led.NUM_ROWS
        self.send_control_data(2, [temp & 255]+[(temp >> 8) & 255]+
                         [(temp >> 16) & 255] + [(temp >> 24) & 255]);
        print 'time_between', temp

    def synchronize(self):
        """ synchronizes the board over usb, with synchro data """
        self.synchronize = True
