#include "stm32f4xx.h"
#include <math.h>

// constant definitions
#define SAFE    1               // temperature in the range
#define UNSAFE  0               // temperature not in the range
#define COOL    0               // to initiate cooling
#define HEAT    1               // to initiate heating

// resistance to conversion constants
#define R_100k  100000
#define B       3950
#define K25C    298.15                  // 25C

// temperature struct
typedef struct {
  float temp;                   // temp reading
  float diff;                   // difference, if the temp is beyond the range
  int flag;                     // SAFE or UNSAFE
}tempStruct;

/**
       LOOKUP TABLE for duty cycle
=========================================
=       Temp diff       =       Duty    =
=         0-0.5         =       20%     =
=         0.6-1         =       40%     =
=        1.1-1.5        =       60%     =
=         1.6-2         =       80%     =
=     2.1 and above     =      100%     =
=========================================
*/


// temperature.c functions
void adc_config();
float get_temperature();
void check_temperature(tempStruct *);
int check_device(tempStruct *);

// sd_card.c functions
void sd_card_config();
void save(tempStruct *);

// controller.c functions
void controllers_config();
void control_temp(tempStruct *);
int get_duty_cycle(float);