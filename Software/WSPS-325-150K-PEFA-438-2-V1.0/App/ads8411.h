#ifndef __ADS8411_H__
#define __ADS8411_H__

#include "main.h"
#include "task.h"

uint16_t get_ADS8411Data_A(void);
uint16_t get_ADS8411Data_B(void);
void ads8411_reset_A(void);
void ads8411_reset_B(void);

#endif


