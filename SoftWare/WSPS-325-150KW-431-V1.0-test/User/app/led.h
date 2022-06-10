#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

void PPT_SW_init(void);
void led_init(void);
void LED_COML(void);

void LED_OFF(uint16_t name);
void LED_ON(uint16_t name);
void PPT_SW(u8 sw);

#define ON 1
#define OFF 0

#define RFOUT_LED 1
#define CURR_LED 2
#define COMM_LED 3
#define TEMP_LED 4
#define VSWR_LED 5
#define POWER_LED 6

#endif /* __I2C_EE_H */
