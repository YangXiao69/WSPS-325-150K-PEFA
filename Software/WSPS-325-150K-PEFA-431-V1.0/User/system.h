#ifndef __SYSTEM_H
#define __SYSTEM_H

#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "w5500.h"
#include "W5500_conf.h"
#include "socket.h"
#include "utility.h"
#include "ds18b20.h"
#include "calibration.h"
#include "gpio.h"
#include "udp.h"
#include "memory.h"
#include "./bsp/bsp.h"

void error_serve(void);


#endif
