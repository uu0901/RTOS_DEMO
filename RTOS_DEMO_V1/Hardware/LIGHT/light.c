#include "light.h"
#include "delay.h"

 
//��ʼ������������
void Light_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTFʱ��	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//PF8 anolog����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	Adc3_Init();
}
//��ȡLight Sens��ֵ
//0~100:0,�;100,���� 
//����Խ��  ADֵԽ��
//����Խǿ  ADֵԽ��
uint8_t Light_Get_Val(void)
{
	uint32_t temp_val=0;
	uint8_t t;
	for(t=0;t<LSENS_READ_TIMES;t++)
	{
		temp_val+=Get_Adc3(LSENS_ADC_CHX);	//��ȡADCֵ
		Delay_ms(5);
	}
	temp_val/=LSENS_READ_TIMES;//�õ�ƽ��ֵ 
	if(temp_val>4000)temp_val=4000;
	return (uint8_t)(100-(temp_val/40));
}












