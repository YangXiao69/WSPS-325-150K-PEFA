
#ifndef __BSP_LED_H
#define	__BSP_LED_H

#include "stm32f4xx.h"

void LED_INIT(void);
void LED_CONTROL(uint16_t name);
void LED_COMM_COML(void);

#define ON  1
#define OFF 0

#define RF_OFF_LED  0
#define RF_RED_LED  1
#define RF_GRE_LED  2
#define RF_YEL_LED  3

#define COMM_OFF_LED  4
#define COMM_RED_LED  5
#define COMM_GRE_LED  6
#define COMM_YEL_LED  7

#define ALARM_OFF_LED  8
#define ALARM_RED_LED  9
#define ALARM_GRE_LED  10
#define ALARM_YEL_LED  11

#endif  
