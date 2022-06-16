#include "system.h"


void Memory_Data(void)
{

    System.K_value = AT24CXX_ReadLenByte(0, 2);
    if (System.K_value > 10000)
        System.K_value = 10000;
    System.K_value2 = AT24CXX_ReadLenByte(2, 2);
    if (System.K_value2 > 10000)
        System.K_value2 = 10000;
    System.K_value3 = AT24CXX_ReadLenByte(4, 2);
    if (System.K_value3 > 10000)
        System.K_value3 = 10000;

}



