#ifndef __LSENS_H
#define __LSENS_H	
#include "sys.h" 
#include "adc.h" 

  
#define LSENS_READ_TIMES	10		//���������������ȡ����,����ô���,Ȼ��ȡƽ��ֵ
#define LSENS_ADC_CHX		ADC_Channel_6	//����������������ڵ�ADCͨ�����
    
void Light_Init(void); 				//��ʼ������������
uint8_t Light_Get_Val(void);				//��ȡ������������ֵ
#endif 





















