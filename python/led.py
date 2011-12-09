import usb.core
import usb.util

LEDS_PER_BOARD = 64
BOARDS = 2
PWM_STEPS = 15
NUM_COLORS = 3
NUM_ROWS = 1000
ROW = BOARDS*LEDS_PER_BOARD*NUM_COLORS

# mapping of pixels, because the ST board has really strange pin orders
LED_ARR = [
    26, # PE10
    25, # PE9
    24, # PE8
    23, # PE7
    49, # PG1
    48, # PG0
    47, # PF15
    46, # PF14
    45, # PF13
    44, # PF12
    43, # PF11
    42, # PF10
    41, # PF9
    40, # PF8
    39, # PF7
    38, # PF6
    37, # PF5
    36, # PF4
    35, # PF3
    34, # PF2
    33, # PF1
    32, # PF0
    63, # PG15
    19, # PE3
    20, # PE4
    21, # PE5
    22, # PE6
    18, # PE2
    16, # PE0
    17, # PE1
    62, # PG14
    61, # PG13
    60, # PG12
    59, # PG11
     0, # PD0
     1, # PD1
     2, # PD2
     3, # PD3
     4, # PD4
     5, # PD5
     6, # PD6
     7, # PD7
    58, # PG10
    57, # PG9
    56, # PG8
    55, # PG7
    54, # PG6
    53, # PG5
    52, # PG4
    51, # PG3
    50, # PG2
    15, # PD15
    14, # PD14
    13, # PD13
    12, # PD12
    11, # PD11
     8, # PD8
     9, # PD9
    10, # PD10
    27, # PE11
    28, # PE12
    29, # PE13
    30, # PE14
    31, # PE15
]

import os
os.environ['PYUSB_LOG_FILENAME'] = 'test.log'
#os.environ['PYUSB_DEBUG_LEVEL'] = 'debug'

import datetime

def divideTimeDelta(td1, td2):
    us1 = td1.microseconds + 1000000 * (td1.seconds + 86400 * td1.days)
    us2 = td2.microseconds + 1000000 * (td2.seconds + 86400 * td2.days)
    return float(us1) / us2

# main script!
def init(find_all=False):
    global dev
    dev = usb.core.find(idVendor=0xffff, idProduct=0x5740, find_all = find_all)

    if dev is None:
        raise ValueError('Device not found')

    # set the active configuration. With no arguments, the first
    # configuration will be the active one
    if find_all:
        for d in dev:
            d.set_configuration()
    else:
        dev.set_configuration()
    return dev

"""
print 'configs:'
for cfg in dev:
    print cfg.bConfigurationValue
    for intf in cfg:
        print '\t' + str(intf.bInterfaceNumber) + ',' + str(intf.bAlternateSetting)
        for ep in intf:
            print '\t\t' + str(ep.bEndpointAddress)
"""

def run(dev, package=None):
    if package is None:
        package = [0]*360*128
        #package = (([1]+[0]*7)*15*3)*128
        #package = ([255]*360)*128
        #package = ([1]*3+[0]*46077)
    # write the data out to the device
    count = 0
    now = datetime.datetime.now()
    report_all = 1

    #while count < 10:
    wrote = dev.write(3, package, 1)

    count += 1
    if count % report_all == 0:
        print count, ',', \
            report_all*divideTimeDelta(datetime.timedelta(seconds=1),\
            datetime.datetime.now()-now),\
            'packages/s, wrote', wrote, 'bytes'
        now = datetime.datetime.now()


    #usb.util.release_interface(dev, 1)
    #usb.util.dispose_resources(dev)

def pwm_data(img, board_nr=0):
    board = 0

    buf = bytearray()
    for i in range(NUM_ROWS):
        index = int(i*ROW+ROW/2*board_nr)
        relevant = sort_led_pixels(img[index:index+int(ROW/2)])
        #print list(relevant)
        #print ROW, index, LEDS_PER_BOARD

        # pwm calculation
        pwm = bytearray(int(PWM_STEPS * LEDS_PER_BOARD * NUM_COLORS / 8))
        for j, pixel in enumerate(relevant):
            pixel = pixel >> 4
            for k in range(PWM_STEPS):
                if k < pixel:
                    # everything in bits -> caution
                    # get basic address for color
                    address = LEDS_PER_BOARD*PWM_STEPS*(j % NUM_COLORS)
                    address += (k)*LEDS_PER_BOARD
                    # add led id (0-LEDS_PER_BOARD)
                    address += int(j/NUM_COLORS)
                    #print 'a', address, PWM_STEPS, LEDS_PER_BOARD, j
                    # 360 bytes address -> 2880 bits
                    pwm[int(address/8)] += 1 << (address % 8)
                else:
                    break

        buf += pwm

        if i > 127:
            break

    return buf[:360*128]

def sort_led_pixels(row): # RGB
    if len(row) != LEDS_PER_BOARD*3:
        raise IndexError(
            'Your software does not deliver the right amount of pixels: '
            + str(len(row)))

    result = bytearray(len(row));
    for i in range(64):
        index = LED_ARR[i]
        result[index*3]   = row[i*3]
        result[index*3+1] = row[i*3+1]
        result[index*3+2] = row[i*3+2]

    #return row
    return result
