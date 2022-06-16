#ifndef __SETTINGS_H
#define __SETTINGS_H

#include "stm32l4xx.h"


extern uint8_t CalibrationFlag;
extern uint8_t BasicVF;
extern uint8_t BasicVR;
extern uint16_t K_value;
extern uint16_t CalibrationPower;



void read_calibration(void);





#endif

