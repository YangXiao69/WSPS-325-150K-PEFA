#include "./../bsp.h"


uint8_t PPT     = 0;


#define ADDR_IN_1 HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9)
#define ADDR_IN_2 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8)
#define ADDR_IN_3 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9)
#define ADDR_IN_4 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10)
#define ADDR_IN_5 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11)
#define ADDR_IN_6 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12)


void BoardAddr_Init(void)
{
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
     
    
      /*Configure GPIO pin   */
    GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_11 | GPIO_PIN_10 | GPIO_PIN_9 | GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    
      /*Configure GPIO pin   */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);    
    

	if(ADDR_IN_1 == 0)
	{
		NumName += 1*1;
	}
	if(ADDR_IN_2 == 0)
	{
		NumName += 2*1;
	}
	if(ADDR_IN_3 == 0)
	{
		NumName += 4*1;
	}
	if(ADDR_IN_4 == 0)
	{
		NumName += 8*1;
	}
	if(ADDR_IN_5 == 0)
	{
		NumName += 16*1;
	}
	if(ADDR_IN_6 == 0)
	{
		NumName += 32*1;
	}

	if(NumName == 0)
	{
		NumName = 1;
	}
}



void PPT_SW_Init(void)
{
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};


    __HAL_RCC_GPIOC_CLK_ENABLE();    
    
       
    GPIO_InitStruct.Pin = GPIO_PIN_15 | GPIO_PIN_13;
//    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);    
    
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);    
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);     
       
}

void PPT_SW(uint8_t sw)
{
    if(sw)  
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
        PPT = 1;
        
    }
    else    
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
        PPT = 0;
    }
}

















