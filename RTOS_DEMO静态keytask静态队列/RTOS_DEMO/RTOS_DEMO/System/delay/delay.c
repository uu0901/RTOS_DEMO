#include "delay.h"
/* ��ӹ���ͷ�ļ� (FreeRTOS ��Ҫ�õ�) */
#include "FreeRTOS.h"
#include "task.h"


static u8  fac_us=0;							//us��ʱ������			   

 
extern void xPortSysTickHandler(void); 
//systick�жϷ�����,ʹ��ucosʱ�õ�
void SysTick_Handler(void)
{	
	/* OS ��ʼ����,��ִ�������ĵ��ȴ��� */
     if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
     {
        xPortSysTickHandler();
     }
}


			   
//��ʼ���ӳٺ���
//��ʹ��OS��ʱ��,�˺������ʼ��OS��ʱ�ӽ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void Delay_Init()
{
	u32 reload;
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);//ѡ���ⲿʱ��  HCLK
	fac_us=SystemCoreClock/1000000;				//�����Ƿ�ʹ��OS,fac_us����Ҫʹ��
	reload=SystemCoreClock/1000000;				//ÿ���ӵļ������� ��λΪM  
	reload*=1000000/configTICK_RATE_HZ;			//����configTICK_RATE_HZ�趨���ʱ��
												//reloadΪ24λ�Ĵ���,���ֵ:16777216,��72M��,Լ��0.233s����	
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//����SYSTICK�ж�
	SysTick->LOAD=reload; 						//ÿ1/configTICK_RATE_HZ���ж�һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//����SYSTICK    

}								    


//��ʱnus
//nus:Ҫ��ʱ��us��.	
//nus:0~204522252(���ֵ��2^32/fac_us@fac_us=168)	    								   
void Delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				//LOAD��ֵ	    	 
	ticks=nus*fac_us; 						//��Ҫ�Ľ����� 
	told=SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};										    
} 
void Delay_ms(uint16_t nms)
{
	uint32_t i;
	
	for (i=0; i<nms; i++)
	{
		Delay_us(1000);
	}
}







































