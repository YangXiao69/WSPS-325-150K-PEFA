#ifndef __SETTINGS_H
#define	__SETTINGS_H

#include <stdio.h>
#include "stm32f4xx.h"



#define CIRBUFF_SIZE 1500

typedef struct tag_cirbuff
{
    __IO uint16_t head;   
    __IO uint16_t tail;  
    __IO uint8_t  tag;  
    __IO uint16_t size;  
    __IO uint16_t buff[CIRBUFF_SIZE];  
}T_CirBuffer;



extern T_CirBuffer cp;


void CirBuff_Init(T_CirBuffer *cirbuff);


int8_t CirBuff_Push(T_CirBuffer *cirbuff, uint16_t *data);
int8_t CirBuff_Pop(T_CirBuffer *cirbuff, uint16_t *data);


uint8_t CirBuff_IsEmpty(T_CirBuffer *cirbuff);
uint8_t CirBuff_IsFull(T_CirBuffer *cirbuff);



#define print_cirq(q) printf("ring head %d,tail %d,tag %d\n", q->head,q->tail,q->tag);

















void log_alarm(void);
void log_operate(void);
void read_ip_24c02(void);
void store_to_24c02(void);
void read_from_24c02(void);
void store_alarm(void);

void store_ip_24c02(void);
//void read_parameter(void);
//void store_parameter(void);


void ReadySend(uint16_t modbusreg);


#endif 

