#include "stm32f10x.h"

extern u8 CalibrationFlag;
extern u8 BasicVF;
extern u8 BasicVR;
extern u16 K_value;
extern u16 K_value2;
extern u16 K_value3;
extern u16 CalibrationPower;
u8 calibration_votage(u16 data, u8 value);
u16 calibration_P(float data, u16 power);
void read_calibration(void);
void calibration_task(void);
