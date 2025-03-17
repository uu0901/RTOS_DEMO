#include "demo_task.h"

//空闲任务配置
StaticTask_t idle_task_tcb;
StackType_t  idle_task_stack[configMINIMAL_STACK_SIZE];
//空闲任务内存分配
void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    StackType_t ** ppxIdleTaskStackBuffer,
                                    uint32_t * pulIdleTaskStackSize )
{
    * ppxIdleTaskTCBBuffer = &idle_task_tcb;
    * ppxIdleTaskStackBuffer = idle_task_stack;
    * pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
//软件定时器任务配置
StaticTask_t timer_task_tcb;
StackType_t  timer_task_stack[configTIMER_TASK_STACK_DEPTH];
//软件定时器内存分配
void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                    StackType_t ** ppxTimerTaskStackBuffer,
                                    uint32_t * pulTimerTaskStackSize )
{
    * ppxTimerTaskTCBBuffer = &timer_task_tcb;
    * ppxTimerTaskStackBuffer = timer_task_stack;
    * pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}


/* LED_Turn任务配置
 * 包括: 任务优先级 堆栈大小 任务句柄 创建任务
 */
#define LED_TURNTASK_PRIO         16
#define LED_TURN_STACK_SIZE       128
TaskHandle_t LED_TurnTask_handler;
StackType_t   LEDTurn_stack[LED_TURN_STACK_SIZE];
StaticTask_t  LEDTurn_tcb;
void LED_Turn_task(void *pvParameters);

/* Motor_Speed任务配置
 * 包括: 任务优先级 堆栈大小 任务句柄 创建任务
 */
#define MotorSpeed_TASK_PRIO         16
#define MotorSpeed_STACK_SIZE       128
TaskHandle_t MotorSpeed_Task_handler;
StackType_t   MotorSpeed_stack[MotorSpeed_STACK_SIZE];
StaticTask_t MotorSpeed_tcb;
void MotorSpeed_task(void *pvParameters);

/* AHT20任务配置
 * 包括: 任务优先级 堆栈大小 任务句柄 创建任务
 */
#define Aht20_TASK_PRIO         16
#define Aht20_STACK_SIZE       128
TaskHandle_t Aht20_Task_handler;
StackType_t   Aht20_stack[Aht20_STACK_SIZE];
StaticTask_t Aht20_tcb;
void Aht20_task(void *pvParameters);


/* AHT20任务配置
 * 包括: 任务优先级 堆栈大小 任务句柄 创建任务
 */
#define Light_TASK_PRIO         16
#define Light_STACK_SIZE       128
TaskHandle_t Light_Task_handler;
StackType_t   Light_stack[Light_STACK_SIZE];
StackType_t  Light_tcb;
void Light_task(void *pvParameters);

extern TaskHandle_t  Start_task_handler;
void Start_Task(void * pvParameters)
{
    taskENTER_CRITICAL();//进入临界区，创建任务完成后，才开始执行优先级任务 

	LED_TurnTask_handler    = xTaskCreateStatic((TaskFunction_t         )LED_Turn_task,
                                                (char *                 ) "LED_Turn_task",
                                                (uint32_t               )LED_TURN_STACK_SIZE,
                                                (void *                 )NULL,
                                                (UBaseType_t            )LED_TURNTASK_PRIO,
                                                (StackType_t *          )LEDTurn_stack,
                                                (StaticTask_t *         )&LEDTurn_tcb);	
                                                
	MotorSpeed_Task_handler = xTaskCreateStatic((TaskFunction_t         )MotorSpeed_task,
                                                (char *                 ) "MotorSpeed_task",
                                                (uint32_t               )MotorSpeed_STACK_SIZE,
                                                (void *                 )NULL,
                                                (UBaseType_t            )MotorSpeed_TASK_PRIO,
                                                (StackType_t *          )MotorSpeed_stack,
                                                (StaticTask_t *         )&MotorSpeed_tcb);
    Aht20_Task_handler =        xTaskCreateStatic((TaskFunction_t         )Aht20_task,
                                                (char *                 ) "Aht20_task",
                                                (uint32_t               )Aht20_STACK_SIZE,
                                                (void *                 )NULL,
                                                (UBaseType_t            )Aht20_TASK_PRIO,
                                                (StackType_t *          )Aht20_stack,
                                                (StaticTask_t *         )&Aht20_tcb);
    Light_Task_handler =        xTaskCreateStatic((TaskFunction_t         )Light_task,
                                                (char *                 ) "Light_task",
                                                (uint32_t               )Light_STACK_SIZE,
                                                (void *                 )NULL,
                                                (UBaseType_t            )Light_TASK_PRIO,
                                                (StackType_t *          )Light_stack,
                                                (StaticTask_t *         )&Light_tcb);
    vTaskDelete(Start_task_handler);//删除开始任务，删除自身用NULL
	taskEXIT_CRITICAL();//退出临界区		
}
uint8_t KeyNum;
void LED_Turn_task(void *pvParameters)
{
    static uint8_t mode = 1;
	while(1)
	{	
        printf("\r\nLED_Turn_task!\r\n");
        KeyNum = Key_GetNum();
        switch(KeyNum)
        {
            case KEY1_SingleClick: 
                                        {
                                            printf("\r\n按键1单击\r\n");
                                        }
                                        break;
            case KEY1_DobleClick: 
                                        {
                                            printf("\r\n按键1双击\r\n");
                                        }break;
            case KEY1_LongCick: 
                                        {
                                            printf("\r\n按键1长按\r\n");              
                                        }break;
            case KEY2_SingleClick: 
                                        {
                                            printf("\r\n按键2单击\r\n");
                                            /* code */
                                            mode = 3;
                                            /* code */
                                        }break;
            case KEY2_DobleClick: 
                                        {
                                            printf("\r\n按键2双击\r\n");
                                            /* code */
                                            mode = 1;
                                            /* code */
                                        }break;
            case KEY2_LongCick: 
                                        {
                                            printf("\r\n按键2长按\r\n");              
                                            printf("\r\n按键2双击\r\n");
                                            /* code */
                                            mode = 2;
                                            /* code */
                                        }break;
            case KEY3_SingleClick: 
                                        {
                                            printf("\r\n按键3单击\r\n");
                                        }break;
            case KEY3_DobleClick: 
                                        {
                                            printf("\r\n按键3双击\r\n");
                                        }break;
            case KEY3_LongCick: 
                                        {
                                            printf("\r\n按键3长按\r\n");              
                                        }break;
            default: break;
        }
        LED1_SetMode(mode);
        LED2_SetMode(mode);
        OLED_Update();
		vTaskDelay(500);
	}
}

void MotorSpeed_task(void *pvParameters)
{
    static uint8_t t;
	while(1)
	{	
        KeyNum = Key_GetNum();
        switch(KeyNum)
        {
            case KEY1_SingleClick: 
                                        {
                                            printf("\r\n按键1单击\r\n");
                                            /* code */
                                            t ++;
                                            if(t>1) t = 0;
                                            if(t == 1)
                                            {
                                                printf("\r\n启动\r\n");
                                                Motor_SetSpeed(90);
                                            }
                                            else
                                            {
                                                printf("\r\n停止\r\n");
                                                Motor_SetSpeed(0);
                                            }
                                            /* code */
                                        }
                                        break;
            case KEY1_DobleClick: 
                                        {
                                            printf("\r\n按键1双击\r\n");
                                        }break;
            case KEY1_LongCick: 
                                        {
                                            printf("\r\n按键1长按\r\n");              
                                        }break;
            case KEY2_SingleClick: 
                                        {
                                            printf("\r\n按键2单击\r\n");
                                        }break;
            case KEY2_DobleClick: 
                                        {
                                            printf("\r\n按键2双击\r\n");
                                        }break;
            case KEY2_LongCick: 
                                        {
                                            printf("\r\n按键2长按\r\n");              
                                        }break;
            case KEY3_SingleClick: 
                                        {
                                            printf("\r\n按键3单击\r\n");
                                        }break;
            case KEY3_DobleClick: 
                                        {
                                            printf("\r\n按键3双击\r\n");
                                        }break;
            case KEY3_LongCick: 
                                        {
                                            printf("\r\n按键3长按\r\n");              
                                        }break;
            default: break;
        }
	}
}

void Aht20_task(void *pvParameters)
{    
    static float temperature;
    static float humidity;
    while(1)
    {
        //--no-multibyte-chars
        atk_aht20_read_data(&temperature, &humidity);       /* 读取ATH20传感器数据 */
        printf("\r\n温度: %.2f℃\r\n", temperature);            /* 计算得到湿度值 */
        printf("\r\n湿度: %.2f%%\r\n", humidity);               /* 计算得到温度值 */
        OLED_ClearArea(49,0,8,16);
        OLED_ClearArea(49,17,8,16);
        OLED_ShowFloatNum(49, 0, temperature, 2, 1, OLED_8X16);
        OLED_ShowChinese(92,0,"℃");
        OLED_ShowFloatNum(49, 17, humidity, 2, 1, OLED_8X16);
        OLED_ShowString(92,17, "%RH", OLED_8X16);

        vTaskDelay(800);
    }
}
void Light_task(void *pvParameters)
{    
    static uint8_t Light;
    while(1)
    {
        Light = Light_Get_Val();
        printf("\r\nLight=%d\r\n", Light);               /* 计算得到温度值 */
        OLED_ShowNum(58, 33, Light, 2, OLED_8X16);
		vTaskDelay(1000);
    }
}
