#include <stdlib.h>

#define LEDS_PER_BOARD    64
#define BOARDS            2
#define PWM_STEPS         2
#define NUM_COLORS        3
#define NUM_ROWS          344
#define ROW               BOARDS*LEDS_PER_BOARD*NUM_COLORS
#define ROW_4BIT          ROW / 2
#define ROW_2BIT          ROW / 4
#define PWM_BOARD_ROW_SIZE      PWM_STEPS * LEDS_PER_BOARD * NUM_COLORS / 8
#define PWM_IMAGE_SIZE          PWM_BOARD_ROW_SIZE*BOARDS*NUM_ROWS
//extern "C" void pwm_data_4bit(unsigned char* img, unsigned char board_nr, unsigned char* pwm_data);

// mapping of pixels, because the ST board has really strange pin orders
unsigned char LED_ARR[] = {
    26, // PE10
    25, // PE9
    24, // PE8
    23, // PE7
    49, // PG1
    48, // PG0
    47, // PF15
    46, // PF14
    45, // PF13
    44, // PF12
    43, // PF11
    42, // PF10
    41, // PF9
    40, // PF8
    39, // PF7
    38, // PF6
    37, // PF5
    36, // PF4
    35, // PF3
    34, // PF2
    33, // PF1
    32, // PF0
    63, // PG15
    19, // PE3
    20, // PE4
    21, // PE5
    22, // PE6
    18, // PE2
    16, // PE0
    17, // PE1
    62, // PG14
    61, // PG13
    60, // PG12
    59, // PG11
     0, // PD0
     1, // PD1
     2, // PD2
     3, // PD3
     4, // PD4
     5, // PD5
     6, // PD6
     7, // PD7
    58, // PG10
    57, // PG9
    56, // PG8
    55, // PG7
    54, // PG6
    53, // PG5
    52, // PG4
    51, // PG3
    50, // PG2
    15, // PD15
    14, // PD14
    13, // PD13
    12, // PD12
    11, // PD11
     8, // PD8
     9, // PD9
    10, // PD10
    27, // PE11
    28, // PE12
    29, // PE13
    30, // PE14
    31, // PE15
};

void sort_led_pixels(unsigned char* row, unsigned char* result){
  // incoming: 64 4bit numbers -> outgoing: 64 8bit numbers
  unsigned char index;
  unsigned int i_new, i;
  for(i=0;i<LEDS_PER_BOARD;i++){
    index = LED_ARR[i];
    i_new = i / 2;
    if (i & 1){
      // second pixel
      result[index*3]   = row[i_new*3+1] >> 4;
      result[index*3+1] = row[i_new*3+2] & 15;
      result[index*3+2] = row[i_new*3+2] >> 4;
    }else{
      // first pixel
      result[index*3]   = row[i_new*3] & 15;
      result[index*3+1] = row[i_new*3] >> 4;
      result[index*3+2] = row[i_new*3+1] & 15;
    }
  }
}

/*
void sort_led_pixels_2bit(unsigned char* row, unsigned char* result){
  // incoming: 64 4bit numbers -> outgoing: 64 8bit numbers
  unsigned char index;
  unsigned int i_new, i;
  for(i=0;i<LEDS_PER_BOARD;i++){
    index = LED_ARR[i];
    i_new = i / 4;
    if (i % 4 == 3 ){
      result[index*3]   = (row[i_new*3+2] >> 2) & 3;
      result[index*3+1] = (row[i_new*3+2] >> 4) & 3;
      result[index*3+2] = (row[i_new*3+2] >> 6) & 3;
    }else if (i % 4==2){
      result[index*3]   = (row[i_new*3+1] >> 4) & 3;
      result[index*3+1] = (row[i_new*3+1] >> 6) & 3;
      result[index*3+2] =  row[i_new*3+2]       & 3;
    }else if (i % 4==1){
      // second pixel
      result[index*3]   = (row[i_new*3  ] >> 6) & 3;
      result[index*3+1] =  row[i_new*3+1]       & 3;
      result[index*3+2] = (row[i_new*3+1] >> 2) & 3;
    }else{
      // first pixel
      result[index*3]   = row[i_new*3   ]       & 3;
      result[index*3+1] = (row[i_new*3  ] >> 2) & 3;
      result[index*3+2] = (row[i_new*3  ] >> 4) & 3;
    }
  }
}
*/
//
#define SHIFT_STEP(shift) \
       targetLed = LED_ARR[targetIndex / NUM_COLORS]; \
       colorOffset = targetIndex % NUM_COLORS;    \
       \
       result[targetLed * 3 + colorOffset] = (value >> shift) & 3; \
       targetIndex++


void sort_led_pixels_2bit(unsigned char* row, unsigned char* result) {
   // incoming: 64 4bit numbers -> outgoing: 64 8bit numbers
   int i;
   int targetIndex = 0;
   unsigned char value;
   int targetLed;

   int colorOffset;

   for (i = 0; i < LEDS_PER_BOARD * NUM_COLORS / 4; i++) {
       value = row[i];

       SHIFT_STEP(6);
       SHIFT_STEP(4);
       SHIFT_STEP(2);
       SHIFT_STEP(0);
   }
}


void pwm_data_4bit_c(unsigned char* img, unsigned char board_nr, unsigned char* pwm_data){
  int index;
  unsigned char relevant[LEDS_PER_BOARD*NUM_COLORS];
  unsigned char pixel;
  int address;
  int i,j,k, temp;
  unsigned char* pwm;

  pwm = pwm_data;
  for(i=0;i<NUM_ROWS;i++){
    index = i*ROW_4BIT+ROW_4BIT/BOARDS*board_nr;
    sort_led_pixels(& img[index], relevant);

    // pwm calculation
    // pixel should already have the right size - because its 4 bit
    for(j=0;j<LEDS_PER_BOARD*NUM_COLORS;j++){
      temp = j % NUM_COLORS;
      pixel = relevant[j];
      // change j because it has wrong values (4bit)
      for(k=0;k<PWM_STEPS;k++){
        if (k < pixel){
          address = LEDS_PER_BOARD*PWM_STEPS*temp;
          address += k*LEDS_PER_BOARD;
          address += j/NUM_COLORS;
          // 360 bytes address -> 2880 bits
          pwm[address/8] += 1 << (address & 7);
        }else{
          break;
        }
      }
    }
    pwm += PWM_BOARD_ROW_SIZE;
  }
}

void pwm_data_2bit_c(unsigned char* img, unsigned char board_nr, unsigned char* pwm_data){
  int index;
  unsigned char relevant[LEDS_PER_BOARD*NUM_COLORS];
  unsigned char pixel;
  int address;
  int i,j,k, temp;
  unsigned char* pwm;

  pwm = pwm_data;
  for(i=0;i<NUM_ROWS;i++){
    index = i*ROW_2BIT+ROW_2BIT/BOARDS*board_nr;
    sort_led_pixels_2bit(& img[index], relevant);

    /*
    if (i == 0){
      for(j=0;j<192;j++){
        printf("%i ",relevant[j]);
        if (j % 3 == 2) printf("\n");
      }
    }
    */

    // clean image -> make zeros
    for (j=0;j<LEDS_PER_BOARD*NUM_COLORS/4;j++){
      pwm[j] = 0;
    }
    // pwm calculation
    // pixel should already have the right size - because its 4 bit
    for(j=0;j<LEDS_PER_BOARD*NUM_COLORS;j++){
      temp = j % NUM_COLORS;
      pixel = relevant[j];
      // change j because it has wrong values (4bit)
      for(k=0;k<PWM_STEPS;k++){
        if (pixel > 0){
          // choose color
          address = LEDS_PER_BOARD*PWM_STEPS*temp;
          // choose pwm step
          address += k*LEDS_PER_BOARD;
          // choose led
          address += j/NUM_COLORS;
          // 48 bytes address -> 384 bits
          pwm[address/8] += 1 << (address & 7);
          pixel = pixel >> 1;
        }else{
          break;
        }
      }
    }
    pwm += PWM_BOARD_ROW_SIZE;
  }
}
