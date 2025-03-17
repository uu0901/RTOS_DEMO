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
#define Aht20_TASK_PRIO         18
#define Aht20_STACK_SIZE        128
TaskHandle_t Aht20_Task_handler;
StackType_t   Aht20_stack[Aht20_STACK_SIZE];
StaticTask_t Aht20_tcb;
void Aht20_task(void *pvParameters);


// /* 亮度任务配置
//  * 包括: 任务优先级 堆栈大小 任务句柄 创建任务
//  */
#define Light_TASK_PRIO         19
#define Light_STACK_SIZE       128
TaskHandle_t Light_Task_handler;
StackType_t   Light_stack[Light_STACK_SIZE];
StaticTask_t  Light_tcb;                        //原先这里变量类型写错StackType_t，修正后所有任务成功运行
void Light_task(void *pvParameters);

/* key任务配置
 * 包括: 任务优先级 堆栈大小 任务句柄 创建任务
 */
#define Key_TASK_PRIO         18
#define Key_STACK_SIZE        128
TaskHandle_t Key_Task_handler;
StackType_t   Key_stack[Key_STACK_SIZE];
StaticTask_t Key_tcb;
void Key_task(void *pvParameters);


/* Music标志任务配置
 * 包括: 任务优先级 堆栈大小 任务句柄 创建任务
 */
#define MusicFlag_TASK_PRIO         16
#define MusicFlag_STACK_SIZE        128
TaskHandle_t MusicFlag_Task_handler;
StackType_t   MusicFlag_stack[MusicFlag_STACK_SIZE];
StaticTask_t MusicFlag_tcb;
void MusicFlag_task(void *pvParameters);
/* Music任务配置
 * 包括: 任务优先级 堆栈大小 任务句柄 创建任务
 */
#define Music_TASK_PRIO         16
#define Music_STACK_SIZE        128
TaskHandle_t Music_Task_handler;
StackType_t   Music_stack[Music_STACK_SIZE];
StaticTask_t Music_tcb;
void Music_task(void *pvParameters);

//定义小数据队列
#define Key_QUEUE_LENGTH    2                   /* 队列支持的消息个数 */
#define Key_QUEUE_ITEM_SIZE sizeof(uint8_t)     /* 队列中每条消息的大小 要匹配你要入队的数据类型*/
uint8_t Key_QUEUE_BUF[Key_QUEUE_LENGTH];
StaticQueue_t Key_pxQueueBuffer;
QueueHandle_t key_Queue;                        //句柄
//定义大数据队列
#define big_data_QUEUE_LENGTH    2                   /* 队列支持的消息个数 */
#define big_data_QUEUE_ITEM_SIZE sizeof(char *)     /* 队列中每条消息的大小  要匹配你要入队的数据类型*/
uint8_t big_data_QUEUE_BUF[big_data_QUEUE_LENGTH];
StaticQueue_t big_data_pxQueueBuffer;
QueueHandle_t big_data_Queue;                   //句柄

char big_data_buff_A[100] = {"我是一个大数组，大大的数组1234 ABCDEF"};
char big_data_buff_B[100] = {"我是猪我是猪"};

extern TaskHandle_t  Start_task_handler;
void Start_Task(void * pvParameters)
{
    taskENTER_CRITICAL();//进入临界区，创建任务完成后，才开始执行优先级任务 

    key_Queue              =    xQueueCreateStatic(Key_QUEUE_LENGTH ,
                                                   Key_QUEUE_ITEM_SIZE , 
                                                   Key_QUEUE_BUF ,
                                                   &Key_pxQueueBuffer);
    if(key_Queue != NULL)
    {
        printf("key_Queue队列创建成功！！\r\n");
    }
    else
    {
        printf("key_Queue队列创建失败！！\r\n");
    }
    big_data_Queue              =  xQueueCreateStatic(big_data_QUEUE_LENGTH , 
                                                    big_data_QUEUE_ITEM_SIZE , 
                                                    big_data_QUEUE_BUF ,
                                                    &big_data_pxQueueBuffer);
    if(big_data_Queue != NULL)
    {
        printf("big_data_Queue队列创建成功！！\r\n");
    }
    else
    {
        printf("big_data_Queue队列创建失败！！\r\n");
    }

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
    Aht20_Task_handler =        xTaskCreateStatic((TaskFunction_t       )Aht20_task,
                                                (char *                 ) "Aht20_task",
                                                (uint32_t               )Aht20_STACK_SIZE,
                                                (void *                 )NULL,
                                                (UBaseType_t            )Aht20_TASK_PRIO,
                                                (StackType_t *          )Aht20_stack,
                                                (StaticTask_t *         )&Aht20_tcb);
    Light_Task_handler =        xTaskCreateStatic((TaskFunction_t       )Light_task,
                                                (char *                 ) "Light_task",
                                                (uint32_t               )Light_STACK_SIZE,
                                                (void *                 )NULL,
                                                (UBaseType_t            )Light_TASK_PRIO,
                                                (StackType_t *          )Light_stack,
                                                (StaticTask_t *         )&Light_tcb);
    Key_Task_handler =        xTaskCreateStatic((TaskFunction_t         )Key_task,
                                                (char *                 ) "Key_task",
                                                (uint32_t               )Key_STACK_SIZE,
                                                (void *                 )NULL,
                                                (UBaseType_t            )Key_TASK_PRIO,
                                                (StackType_t *          )Key_stack,
                                                (StaticTask_t *         )&Key_tcb);
    MusicFlag_Task_handler =  xTaskCreateStatic((TaskFunction_t         )MusicFlag_task,
                                                (char *                 ) "MusicFlag_task",
                                                (uint32_t               )MusicFlag_STACK_SIZE,
                                                (void *                 )NULL,
                                                (UBaseType_t            )MusicFlag_TASK_PRIO,
                                                (StackType_t *          )MusicFlag_stack,
                                                (StaticTask_t *         )&MusicFlag_tcb);
    Music_Task_handler =      xTaskCreateStatic((TaskFunction_t         )Music_task,
                                                (char *                 ) "Music_task",
                                                (uint32_t               )Music_STACK_SIZE,
                                                (void *                 )NULL,
                                                (UBaseType_t            )Music_TASK_PRIO,
                                                (StackType_t *          )Music_stack,
                                                (StaticTask_t *         )&Music_tcb);
    vTaskDelete(Start_task_handler);//删除开始任务，删除自身用NULL

    taskEXIT_CRITICAL();//退出临界区		
}
void LED_Turn_task(void *pvParameters)
{
    uint8_t key = 0;
    BaseType_t err = 0;
    while(1)
    {	    
        printf("\r\nLED_Turn_task!\r\n");
        err = xQueueReceive(key_Queue, &key, portMAX_DELAY);
        if(err != pdTRUE)
        {
            printf("key_Queue队列读取失败！！\r\n");
        }
        else
        {
            printf("key_Queue队列读取成功！！数据：%d\r\n", key);//出队
        }
        if(key == KEY1_SingleClick)
        {
            LED1_SetMode(4);
            LED2_SetMode(4);
        }
        else if(key == KEY1_DobleClick)
        {
            LED1_SetMode(3);
            LED2_SetMode(3);
        }
        else if(key == KEY1_LongCick)
        {
            LED1_SetMode(1);
            LED2_SetMode(1);
        }
        //由于task2任务优先级比task1高，按理说是需要加延时函数进入阻塞，task1才能运行，但这里，我们调用了xQueueReceive函数，此函数如果队列是空的，那么读取不了，
        //所以任务会从就绪态移除，移动到阻塞态，因此task2也就进入阻塞态了，因此就算没有调用延时函数，task2也会进入阻塞态
	}
}

void MotorSpeed_task(void *pvParameters)
{
    char * buf;
    BaseType_t err = 0;
	while(1)
	{	
        printf("\r\nMotorSpeed_task!\r\n");
        err = xQueueReceive(big_data_Queue, &buf, portMAX_DELAY);

        // if(buf == "我是一个大数组，大大的数组1234 ABCDEF")
        // {
        //     LED1_SetMode(4);
        //     LED2_SetMode(4);
        // }在C语言中直接使用 == 比较字符串会比较指针地址而非内容，这是条件判断失效的根本原因。
        // 使用strcmp进行内容比较
        if(err != pdTRUE)
        {
            printf("big_data_Queue队列读取失败！！\r\n");
        }
        else
        {
            printf("big_data_Queue队列读取成功！！数据：%s\r\n", buf);//出队
        }
        if(strcmp(buf, "我是一个大数组，大大的数组1234 ABCDEF") == 0) //strcmp属于<string.h>，必须包含这个头文件。
        {
            LED1_SetMode(4);
            LED2_SetMode(4);
            Motor_SetSpeed(90);
            printf("字符串匹配成功，LED模式已切换！\r\n");
        }
        else if(strcmp(buf, "我是猪我是猪") == 0) 
        {
            LED1_SetMode(1);
            LED2_SetMode(1);
            Motor_SetSpeed(0);
        }

	}
}

void Aht20_task(void *pvParameters)
{    
    static float temperature;
    static float humidity;
    while(1)
    {
        // --no-multibyte-chars
        atk_aht20_read_data(&temperature, &humidity);       /* 读取ATH20传感器数据 */
        printf("\r\n温度: %.2f℃\r\n", temperature);            /* 计算得到湿度值 */
        printf("\r\n湿度: %.2f%%\r\n", humidity);               /* 计算得到温度值 */
        OLED_ClearArea(49,0,8,16);
        OLED_ClearArea(49,17,8,16);
        OLED_ShowFloatNum(49, 0, temperature, 2, 1, OLED_8X16);
        OLED_ShowChinese(92,0,"℃");
        OLED_ShowFloatNum(49, 17, humidity, 2, 1, OLED_8X16);
        OLED_ShowString(92,17, "%RH", OLED_8X16);


        
        // 获取当前任务的堆栈高水位线
        UBaseType_t highWaterMark_words = uxTaskGetStackHighWaterMark(NULL);
        UBaseType_t highWaterMark_bytes = highWaterMark_words * sizeof(StackType_t); // 转换为字节
        printf("Stack High Water Mark: %u bytes\n", highWaterMark_bytes);
        printf("Task running...\n");
        vTaskDelay(1500);
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
// //tsak1任务函数：实现入队
void Key_task(void *pvParameters)
{ 
    uint8_t mykey = 0;
    BaseType_t err = 0;
    char * buf_A;
    char * buf_B;
    buf_A = big_data_buff_A;//等效于buf = &buff[0]
    buf_B = big_data_buff_B;
    while(1)
    {
        // printf("\r\nKey_task!\r\n");
        mykey = Key_GetNum();
        // printf("%d\r\n",mykey);
        if(mykey == KEY1_SingleClick || mykey == KEY1_LongCick || mykey == KEY1_DobleClick || mykey == KEY3_SingleClick || mykey == KEY3_LongCick)
        {
            err = xQueueSend(key_Queue, &mykey, portMAX_DELAY);//portMAX_DELAY最大延时也就是死等
            if(err != pdTRUE)
            {
                printf("key_Queue队列发送失败！！\r\n");
            }
            else
            {
                printf("key_Queue队列发送成功！！\r\n");//入队
            }
        }
        else if(mykey == KEY2_SingleClick)
        {
            err = xQueueSend(big_data_Queue, &buf_A, portMAX_DELAY);//portMAX_DELAY最大延时也就是死等
            if(err != pdTRUE)
            {
                printf("big_data_Queue队列发送失败！！\r\n");
            }
            else
            {
                printf("big_data_Queue队列发送成功！！\r\n");//入队
            }
        }
        else if(mykey == KEY2_DobleClick)
        {
            err = xQueueSend(big_data_Queue, &buf_B, portMAX_DELAY);//portMAX_DELAY最大延时也就是死等
            if(err != pdTRUE)
            {
                printf("big_data_Queue队列发送失败！！\r\n");
            }
            else
            {
                printf("big_data_Queue队列发送成功！！\r\n");//入队
            }
        }
        OLED_Update();
        vTaskDelay(100);//相对延时
    }
}   
uint8_t MusicFlag = 0;
void MusicFlag_task(void *pvParameters)
{   
    uint8_t key = 0;
    BaseType_t err = 0;
    while(1)
    {
        printf("\r\nMusicFlag_task!\r\n");
        err = xQueueReceive(key_Queue, &key, portMAX_DELAY);
        if(err != pdTRUE)
        {
            printf("key_Queue队列读取失败！！\r\n");
        }
        else
        {
            printf("key_Queue队列读取成功！！数据：%d\r\n", key);//出队
        }
        if(key == KEY3_SingleClick)
        {
            MusicFlag = 1;
        }
        else if(key == KEY3_LongCick)
        {
            MusicFlag = 0;
        }
    }
}
void Music_task(void *pvParameters)
{    
    while(1)
    {
        printf("\r\nMusic_task!\r\n");
        if(MusicFlag == 1)          PlayMusic();
        else if(MusicFlag == 0)     Stop_Music();
        vTaskDelay(100);//相对延时
    }
}
