#include "main.h"                  // Device header

static void BSP_Init(void);


#define START_TASK_PRIO          1                      //��ʼ�������ȼ�
#define START_TASK_STACK_SIZE  128                      //��ջ��С
void Start_Task(void * pvParameters);                   //������
TaskHandle_t  Start_task_handler;                       //������
StackType_t   start_task_stack[START_TASK_STACK_SIZE];  //puxStackBuffer��ָ���û�����Ķ�ջ��������StackType_t �������飩��
StaticTask_t  start_task_tcb;                           //pxTaskBuffer��ָ���û������ TCB �ṹ��StaticTask_t ���ͱ�������

int main(void)
{
    BSP_Init();
    Start_task_handler = xTaskCreateStatic( (TaskFunction_t   )   Start_Task,
                                            (char *           )	 "Start_Task",
                                            (uint32_t         )	 START_TASK_STACK_SIZE,
                                            (void *           )	 NULL,
                                            (UBaseType_t      )	 START_TASK_PRIO,	 
                                            (StackType_t *    )	 start_task_stack,	
                                            (StaticTask_t *   )	 &start_task_tcb);
    vTaskStartScheduler();//�����������	
}

static void BSP_Init(void)
{
/*
    *STM32�ж����ȼ�����Ϊ4����4bit��������ʾ��ռ���ȼ�����ΧΪ��0~15 
    ���ȼ�����ֻ��Ҫ����һ�μ��ɣ��Ժ������������������Ҫ�õ��жϣ� 
    ��ͳһ��������ȼ����飬ǧ��Ҫ�ٷ��飬�мɡ� 
*/
    NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4); 
    Delay_Init();
    Uart_Init(115200);  
    Key_Init();
    LED_Init();
    TIM3_Int_Init(1000 -1, 72 -1);
    Motor_Init();
    Aht20_Init();
    Light_Init();
//    Music_init();
    OLED_Init();
    OLED_ShowChinese(0,0,"�¶ȣ�");
    OLED_ShowChinese(0,17,"ʪ�ȣ�");
    OLED_ShowChinese(0,33,"���ȣ�");
    OLED_ShowChinese(0,49,"���֣�");
    OLED_Update();                       //ˢ����Ļ��ʾ
    printf("hello\r\n");
}
