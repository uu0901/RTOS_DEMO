/**
*Encoding:GB2312
*�ļ�����:������Ӳ������ʵ��
**/

#include "stm32f10x.h"                  // Device header
#include "Delay.h"

uint8_t MusicState = 0;             // ȫ�ֱ�������¼���ֲ��ŵ�״̬ 

/**
*��������������ʹ�������ʼ������
*��������
*����ֵ����
**/
void Music_init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);          //����TIM2��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);         //����GPIOA��ʱ��
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                     
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;             //��PA0���ų�ʼ��Ϊ�����������	
	GPIO_Init(GPIOA, &GPIO_InitStructure);                        //��������Ƶ����ţ�����Ҫ����Ϊ����ģʽ	
	TIM_InternalClockConfig(TIM2);                                //ѡ��TIM2Ϊ�ڲ�ʱ�ӣ��������ô˺�����TIMĬ��ҲΪ�ڲ�ʱ��
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 99;	
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1439;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 50;	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	

	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 50;		//CCR
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_Cmd(TIM2, ENABLE);
}

/**
*������������Ƶ������
*��������
*����ֵ����
**/
void Sound_SetHZ(uint16_t a)
{
	TIM_PrescalerConfig(TIM2,a,TIM_PSCReloadMode_Immediate);
}

/**
*��������������
*��������
*����ֵ����
**/
void Play_Music(int a,int b,int c)
{
	Music_init();
	Sound_SetHZ(a);Delay_ms(b);Sound_SetHZ(20);Delay_ms(c);
}

/**
*��������ͣ����
*��������
*����ֵ����
**/
void Stop_Music(void)
{
	TIM_Cmd(TIM2, DISABLE);;  //// �رն�ʱ����ֹͣPWM���
}

