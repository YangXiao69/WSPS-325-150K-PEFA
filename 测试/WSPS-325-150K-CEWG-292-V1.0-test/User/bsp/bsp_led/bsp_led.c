#include "system.h" 



void LED_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOG, ENABLE); 
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;	
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOG, GPIO_Pin_2);	
	GPIO_ResetBits(GPIOG, GPIO_Pin_3);
	GPIO_ResetBits(GPIOG, GPIO_Pin_4);	
	GPIO_ResetBits(GPIOG, GPIO_Pin_5);	
	GPIO_ResetBits(GPIOG, GPIO_Pin_6);	
    GPIO_ResetBits(GPIOG, GPIO_Pin_7);		
}
 
void LED_CONTROL(uint16_t name)
{
    switch(name)
    {
        case 0 : GPIO_ResetBits(GPIOG, GPIO_Pin_2);GPIO_ResetBits(GPIOG, GPIO_Pin_3);break;
        case 1 : GPIO_SetBits  (GPIOG, GPIO_Pin_2);GPIO_ResetBits(GPIOG, GPIO_Pin_3);break;
        case 2 : GPIO_ResetBits(GPIOG, GPIO_Pin_2);GPIO_SetBits  (GPIOG, GPIO_Pin_3);break;
        case 3 : GPIO_SetBits  (GPIOG, GPIO_Pin_2);GPIO_SetBits  (GPIOG, GPIO_Pin_3);break;
        
        case 4 : GPIO_ResetBits(GPIOG, GPIO_Pin_4);GPIO_ResetBits(GPIOG, GPIO_Pin_5);break;
        case 5 : GPIO_SetBits  (GPIOG, GPIO_Pin_4);GPIO_ResetBits(GPIOG, GPIO_Pin_5);break;
        case 6 : GPIO_ResetBits(GPIOG, GPIO_Pin_4);GPIO_SetBits  (GPIOG, GPIO_Pin_5);break;
        case 7 : GPIO_SetBits  (GPIOG, GPIO_Pin_4);GPIO_SetBits  (GPIOG, GPIO_Pin_5);break;
        
        case 8 : GPIO_ResetBits(GPIOG, GPIO_Pin_6);GPIO_ResetBits(GPIOG, GPIO_Pin_7);break;
        case 9 : GPIO_SetBits  (GPIOG, GPIO_Pin_6);GPIO_ResetBits(GPIOG, GPIO_Pin_7);break;
        case 10: GPIO_ResetBits(GPIOG, GPIO_Pin_6);GPIO_SetBits  (GPIOG, GPIO_Pin_7);break;
        case 11: GPIO_SetBits  (GPIOG, GPIO_Pin_6);GPIO_SetBits  (GPIOG, GPIO_Pin_7);break;
        
        default :break;			
    }
}


void LED_COMM_COML(void)
{
    if(GPIO_ReadOutputDataBit(GPIOG, GPIO_Pin_5))
        GPIO_ResetBits(GPIOG, GPIO_Pin_5);
    else 
        GPIO_SetBits(GPIOG, GPIO_Pin_5);		
}

