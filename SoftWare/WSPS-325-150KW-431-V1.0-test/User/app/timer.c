
/**********************************************************************************/
#include "system.h"

u16 timenum = 0;

/*TIM_Period--1000   TIM_Prescaler--71 -->中断周期为1.5ms*/
void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    //    TIM_DeInit(TIM2);
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    TIM_TimeBaseStructure.TIM_Period = 2000;                    /* 自动重装载寄存器周期的值(计数值) */
    TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);             /* 时钟预分频数 72M/72 */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     /* 采样分频 */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; /* 向上计数模式 */
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update); /* 清除溢出中断标志 */
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE); /* 开启时钟 */

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE); /*先关闭等待使用*/
}

/**************************************************************************
         				     *定时器4配置*
**************************************************************************/
void TIM4_Configuration(void) /* 100us */
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能

    TIM_TimeBaseStructure.TIM_Period = 100;                     //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);             //设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);      /* 清除溢出中断标志 */
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); /* 开启定时器中断 */
    TIM_Cmd(TIM4, ENABLE);                     /* 开启时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, DISABLE);
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
        timenum++;
        if (timenum >= 300) /* 250 * 2ms = 500ms */
        {
            timenum = 0;
        }
    }
}

void TIM4_IRQHandler(void) //TIM4中断
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查TIM3更新中断发生与否
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update); //清除TIMx更新中断标志
    }
}

void staet_timer(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_Cmd(TIM2, ENABLE);
    TIM_Cmd(TIM4, ENABLE);
}
