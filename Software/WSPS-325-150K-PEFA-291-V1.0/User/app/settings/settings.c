#include "./../../main.h"


uint8_t BasicVF;       /* 正向基础电压 */
uint8_t BasicVR;       /* 反向基础电压 */
uint16_t K_value;
uint8_t CalibrationFlag=0;
uint16_t CalibrationPower;



void read_calibration(void)
{
    uint8_t readbuff[8] = {0};
    
    I2C_EE_BufferRead(readbuff,250,4);
    
    BasicVF = readbuff[0];
    BasicVR = readbuff[1];
    K_value = readbuff[2];
    K_value = (K_value<<8) + readbuff[3];
    if(K_value<1000)   K_value=1000;
    if(K_value>16000)  K_value=16000;
}


