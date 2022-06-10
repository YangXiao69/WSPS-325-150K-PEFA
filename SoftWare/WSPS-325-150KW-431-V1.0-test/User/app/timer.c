
/**********************************************************************************/
#include "system.h"

u16 timenum = 0;

/*TIM_Period--1000   TIM_Prescaler--71 -->�ж�����Ϊ1.5ms*/
void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    //    TIM_DeInit(TIM2);
    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    TIM_TimeBaseStructure.TIM_Period = 2000;                    /* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);             /* ʱ��Ԥ��Ƶ�� 72M/72 */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     /* ������Ƶ */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; /* ���ϼ���ģʽ */
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update); /* �������жϱ�־ */
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE); /* ����ʱ�� */

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE); /*�ȹرյȴ�ʹ��*/
}

/**************************************************************************
         				     *��ʱ��4����*
**************************************************************************/
void TIM4_Configuration(void) /* 100us */
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��

    TIM_TimeBaseStructure.TIM_Period = 100;                     //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);             //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);      /* �������жϱ�־ */
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); /* ������ʱ���ж� */
    TIM_Cmd(TIM4, ENABLE);                     /* ����ʱ�� */
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

void TIM4_IRQHandler(void) //TIM4�ж�
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���TIM3�����жϷ������
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update); //���TIMx�����жϱ�־
    }
}

void staet_timer(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_Cmd(TIM2, ENABLE);
    TIM_Cmd(TIM4, ENABLE);
}
