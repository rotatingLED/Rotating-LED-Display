import led
import time

# one image ~ 200K
IMAGE_BUFFER_LENGTH = 64
image_current = 0
image_pwm_current = 0
image_buffer = [None]*IMAGE_BUFFER_LENGTH
image_pwm_buffer = [None]*IMAGE_BUFFER_LENGTH

class PwmCalculation(threading.Thread):
    def __init__(self, debug=FALSE):
        threading.Thread.__init__(self)
        self.debug = debug

    def run(self):
        i = 0
        while 1:
            # check if the calculation is too slow -> then 
            if (image_current - IMAGE_BUFFER_LENGTH) >= image_pwm_current:
                # maybe raise an exception?
                print 'Buffer underrun!'
            if image_pwm_current < image_buff:
                index = image_pwm_current % IMAGE_BUFFER_LENGTH
                image_pwm_buffer[index] = led.pwm_data_4bit()
            else:
                time.sleep(0.01)


class Board(threading.Thread):
    def __init__(self, dev):
        threading.Thread.__init__(self)
        self.dev = dev

    def run(self):
        # realtime stuff - the shit!
        i = 0
        while i < 10:
            led.run(self.dev, data)
