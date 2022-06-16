#ifndef __BSP_ADF4351_H  
#define __BSP_ADF4351_H 




void ADF4351_GPIO_Init(void);
void adf4351_write_data(unsigned int register_data);
void Generate_2450MHz_Freq(void);
void adf4351_freq(unsigned int data_number);
void Rf_outer(void);
void Rf_enter(void);
void set_freq(unsigned int data_number);




#endif 
