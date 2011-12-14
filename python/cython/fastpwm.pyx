
cdef extern from "fastpwmc.c":
  void pwm_data_4bit_c(unsigned char* img, 
          unsigned char board_nr, unsigned char* pwm_data)

DEF LEDS_PER_BOARD = 64
DEF BOARDS = 2
DEF PWM_STEPS = 15
DEF NUM_COLORS = 3
DEF NUM_ROWS = 1000
DEF ROW = BOARDS*LEDS_PER_BOARD*NUM_COLORS
DEF ROW_4BIT = ROW >> 1
DEF PWM_BOARD_ROW_SIZE = PWM_STEPS * LEDS_PER_BOARD * NUM_COLORS >> 3
DEF PWM_IMAGE_SIZE     = PWM_BOARD_ROW_SIZE*BOARDS*NUM_ROWS

# mapping of pixels, because the ST board has really strange pin orders
cdef unsigned char LED_ARR[LEDS_PER_BOARD]
LED_ARR[0] = 26 # PE10
LED_ARR[1] = 25 # PE9
LED_ARR[2] = 24 # PE8
LED_ARR[3] = 23 # PE7
LED_ARR[4] = 49 # PG1
LED_ARR[5] = 48 # PG0
LED_ARR[6] = 47 # PF15
LED_ARR[7] = 46 # PF14
LED_ARR[8] = 45 # PF13
LED_ARR[9] = 44 # PF12
LED_ARR[10] = 43 # PF11
LED_ARR[11] = 42 # PF10
LED_ARR[12] = 41 # PF9
LED_ARR[13] = 40 # PF8
LED_ARR[14] = 39 # PF7
LED_ARR[15] = 38 # PF6
LED_ARR[16] = 37 # PF5
LED_ARR[17] = 36 # PF4
LED_ARR[18] = 35 # PF3
LED_ARR[19] = 34 # PF2
LED_ARR[20] = 33 # PF1
LED_ARR[21] = 32 # PF0
LED_ARR[22] = 63 # PG15
LED_ARR[23] = 19 # PE3
LED_ARR[24] = 20 # PE4
LED_ARR[25] = 21 # PE5
LED_ARR[26] = 22 # PE6
LED_ARR[27] = 18 # PE2
LED_ARR[28] = 16 # PE0
LED_ARR[29] = 17 # PE1
LED_ARR[30] = 62 # PG14
LED_ARR[31] = 61 # PG13
LED_ARR[32] = 60 # PG12
LED_ARR[33] = 59 # PG11
LED_ARR[34] =  0 # PD0
LED_ARR[35] =  1 # PD1
LED_ARR[36] =  2 # PD2
LED_ARR[37] =  3 # PD3
LED_ARR[38] =  4 # PD4
LED_ARR[39] =  5 # PD5
LED_ARR[40] =  6 # PD6
LED_ARR[41] =  7 # PD7
LED_ARR[42] = 58 # PG10
LED_ARR[43] = 57 # PG9
LED_ARR[44] = 56 # PG8
LED_ARR[45] = 55 # PG7
LED_ARR[46] = 54 # PG6
LED_ARR[47] = 53 # PG5
LED_ARR[48] = 52 # PG4
LED_ARR[49] = 51 # PG3
LED_ARR[50] = 50 # PG2
LED_ARR[51] = 15 # PD15
LED_ARR[52] = 14 # PD14
LED_ARR[53] = 13 # PD13
LED_ARR[54] = 12 # PD12
LED_ARR[55] = 11 # PD11
LED_ARR[56] =  8 # PD8
LED_ARR[57] =  9 # PD9
LED_ARR[58] = 10 # PD10
LED_ARR[59] = 27 # PE11
LED_ARR[60] = 28 # PE12
LED_ARR[61] = 29 # PE13
LED_ARR[62] = 30 # PE14
LED_ARR[63] = 31 # PE15

def pwm_4bit_c(img, board_nr, pwm_data):
    pwm_data_4bit_c(img, board_nr, pwm_data)

def pwm_data_4bit(unsigned char* img, unsigned char board_nr=0):
    cdef int index
    cdef unsigned char relevant[LEDS_PER_BOARD*NUM_COLORS]
    cdef unsigned char pixel
    cdef int address
    cdef int i,j,k, temp
    buf = bytearray()
    for i in range(NUM_ROWS):
        index = i*ROW_4BIT+ROW_4BIT/BOARDS*board_nr
        #relevant = sort_led_pixels(img[index:index+int(ROW/BOARDS)])
        sort_led_pixels(& img[index], relevant)
        #print list(relevant)
        #print ROW, index, LEDS_PER_BOARD

        # pwm calculation
        # pixel should already have the right size - because its 4 bit
        pwm = bytearray(int(PWM_STEPS * LEDS_PER_BOARD * NUM_COLORS / 8))
        for j in range(LEDS_PER_BOARD*NUM_COLORS):
            temp = j % NUM_COLORS
            pixel = relevant[j]
            # change j because it has wrong values (4bit)
            for k in range(PWM_STEPS):
                if k < pixel:
                    # everything in bits -> caution
                    # get basic address for color
                    address = LEDS_PER_BOARD*PWM_STEPS*temp
                    address += k*LEDS_PER_BOARD
                    temp2 = (address, k)
                    # add led id (0-LEDS_PER_BOARD)
                    address += j/NUM_COLORS
                    #print 'a', address, PWM_STEPS, LEDS_PER_BOARD, j
                    # 360 bytes address -> 2880 bits
                    pwm[address/8] += (1 << (address & 7))
                else:
                    break

        buf += pwm
        if i == 0:
          for l in range(100):
            #print pwm[l]
            pass
    return buf

cdef void sort_led_pixels(unsigned char* row, unsigned char* result): # RGB
    """ incoming: 64 4bit numbers -> outgoing: 64 8bit numbers """
    cdef unsigned char index
    cdef unsigned int i_new, i
    for i in range(LEDS_PER_BOARD):
        index = LED_ARR[i]
        i_new = i / 2
        if i & 1:
            # second pixel
            result[index*3]   = row[i_new*3+1] >> 4
            result[index*3+1] = row[i_new*3+2] & 15
            result[index*3+2] = row[i_new*3+2] >> 4
        else:
            # first pixel
            result[index*3]   = row[i_new*3] & 15
            result[index*3+1] = row[i_new*3] >> 4
            result[index*3+2] = row[i_new*3+1] & 15
