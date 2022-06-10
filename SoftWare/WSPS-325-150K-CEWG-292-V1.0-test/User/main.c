/**
  *********************************************************************************
  * @file    	main.c
  * @author  	 
  * @version 	V1.00
  * @date    	2021-05-29
  * @brief
  *
  *        Model : WSPS-2450-6K-CEWG
  *                               
  *                              
  *          MCU : STM32F429ZET6 
  *     Hardware : WS7.820.292.V03   
  * 
  * @revision
  *     
  *     version         date         author         details      
  *     
  *     
  *                    
  *     
  *     
  * 
  *
  *********************************************************************************
  */ 



#include "system.h" 




int main(void)
{	
    uint16_t i = 0, j = 0;

//	SCB->VTOR = FLASH_BASE | 0x10000;//…Ë÷√∆´“∆¡ø 

    Bsp_Inits();
     
    while(1)
	{	
		Ptt_Control();
        
		for(i=0;i<7;i++)
        {
            delay_us(80);					
            adc_data_calculate();
		}
		j++;

        if(j%20)
		{
            Write_ModbusReg();          
		}
        
        if(j==60)
        {
            j = 0;
//            lcd_send_pagemain();             
            Get_Flow_Temp(); 
        }
        
	   	UDP_Service(); 
		TCP_TopServer();
	}	
}





/*************************************** END OF FILE *********************************************/

