#include "main.h"                  // Device header

static void BSP_Init(void);


#define START_TASK_PRIO          1                      //开始任务优先级
#define START_TASK_STACK_SIZE  128                      //堆栈大小
void Start_Task(void * pvParameters);                   //任务函数
TaskHandle_t  Start_task_handler;                       //任务句柄
StackType_t   start_task_stack[START_TASK_STACK_SIZE];  //puxStackBuffer：指向用户定义的堆栈缓冲区（StackType_t 类型数组）。
StaticTask_t  start_task_tcb;                           //pxTaskBuffer：指向用户定义的 TCB 结构（StaticTask_t 类型变量）。

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
    vTaskStartScheduler();//开启任务调度	
}

static void BSP_Init(void)
{
/*
    *STM32中断优先级分组为4，即4bit都用来表示抢占优先级，范围为：0~15 
    优先级分组只需要分组一次即可，以后如果有其他的任务需要用到中断， 
    都统一用这个优先级分组，千万不要再分组，切忌。 
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
    OLED_ShowChinese(0,0,"温度：");
    OLED_ShowChinese(0,17,"湿度：");
    OLED_ShowChinese(0,33,"亮度：");
    OLED_ShowChinese(0,49,"音乐：");
    OLED_Update();                       //刷新屏幕显示
    printf("hello\r\n");
}
