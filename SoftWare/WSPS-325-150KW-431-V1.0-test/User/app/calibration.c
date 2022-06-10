#include "system.h"

u8 BasicVF; /* 正向基础电压 */
u8 BasicVR; /* 反向基础电压 */
u16 K_value;
u16 K_value2;
u16 K_value3;
u8 CalibrationFlag = 0;
u16 CalibrationPower;

void read_calibration(void)
{
    u8 readbuff[8];
    AT24CXX_Read(250, readbuff, 4);
    BasicVF = readbuff[0];
    BasicVR = readbuff[1];
    K_value = readbuff[2];
    K_value = (K_value << 8) + readbuff[3];
    if (K_value < 300)
        K_value = 300;
    if (K_value > 3000)
        K_value = 3000;
}

void calibration_task(void)
{
    //    if(CalibrationFlag==1)
    //    {
    //        if((ADC_AverageValue[7]*3280/4096)>=2)    BasicVF=(ADC_AverageValue[7]*3280/4096)-2;
    //        else                                      BasicVF=0;

    //        if((ADC_AverageValue[8]*3280/4096)>=2)    BasicVR=(ADC_AverageValue[8]*3280/4096)-2;
    //        else                                      BasicVR=0;

    //        AT24CXX_WriteOneByte(250,BasicVF);
    //        AT24CXX_WriteOneByte(251,BasicVR);
    //        CalibrationFlag = 0;
    //    }
    //    else if(CalibrationFlag==2)
    //    {
    ////			  K_value = ADC_AverageValue[7]*ADC_AverageValue[7];
    ////			  K_value = K_value>>10;
    ////        AT24CXX_WriteTwoByte(252,K_value);
    //        CalibrationFlag = 0;
    //    }
}
