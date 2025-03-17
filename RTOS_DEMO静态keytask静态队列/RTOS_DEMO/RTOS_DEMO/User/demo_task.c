#include "demo_task.h"

//������������
StaticTask_t idle_task_tcb;
StackType_t  idle_task_stack[configMINIMAL_STACK_SIZE];
//���������ڴ����
void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    StackType_t ** ppxIdleTaskStackBuffer,
                                    uint32_t * pulIdleTaskStackSize )
{
    * ppxIdleTaskTCBBuffer = &idle_task_tcb;
    * ppxIdleTaskStackBuffer = idle_task_stack;
    * pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
//�����ʱ����������
StaticTask_t timer_task_tcb;
StackType_t  timer_task_stack[configTIMER_TASK_STACK_DEPTH];
//�����ʱ���ڴ����
void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                    StackType_t ** ppxTimerTaskStackBuffer,
                                    uint32_t * pulTimerTaskStackSize )
{
    * ppxTimerTaskTCBBuffer = &timer_task_tcb;
    * ppxTimerTaskStackBuffer = timer_task_stack;
    * pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}


/* LED_Turn��������
 * ����: �������ȼ� ��ջ��С ������ ��������
 */
#define LED_TURNTASK_PRIO         16
#define LED_TURN_STACK_SIZE       128
TaskHandle_t LED_TurnTask_handler;
StackType_t   LEDTurn_stack[LED_TURN_STACK_SIZE];
StaticTask_t  LEDTurn_tcb;
void LED_Turn_task(void *pvParameters);

/* Motor_Speed��������
 * ����: �������ȼ� ��ջ��С ������ ��������
 */
#define MotorSpeed_TASK_PRIO         16
#define MotorSpeed_STACK_SIZE       128
TaskHandle_t MotorSpeed_Task_handler;
StackType_t   MotorSpeed_stack[MotorSpeed_STACK_SIZE];
StaticTask_t MotorSpeed_tcb;
void MotorSpeed_task(void *pvParameters);

/* AHT20��������
 * ����: �������ȼ� ��ջ��С ������ ��������
 */
#define Aht20_TASK_PRIO         18
#define Aht20_STACK_SIZE        128
TaskHandle_t Aht20_Task_handler;
StackType_t   Aht20_stack[Aht20_STACK_SIZE];
StaticTask_t Aht20_tcb;
void Aht20_task(void *pvParameters);


// /* ������������
//  * ����: �������ȼ� ��ջ��С ������ ��������
//  */
#define Light_TASK_PRIO         19
#define Light_STACK_SIZE       128
TaskHandle_t Light_Task_handler;
StackType_t   Light_stack[Light_STACK_SIZE];
StaticTask_t  Light_tcb;                        //ԭ�������������д��StackType_t����������������ɹ�����
void Light_task(void *pvParameters);

/* key��������
 * ����: �������ȼ� ��ջ��С ������ ��������
 */
#define Key_TASK_PRIO         18
#define Key_STACK_SIZE        128
TaskHandle_t Key_Task_handler;
StackType_t   Key_stack[Key_STACK_SIZE];
StaticTask_t Key_tcb;
void Key_task(void *pvParameters);


/* Music��־��������
 * ����: �������ȼ� ��ջ��С ������ ��������
 */
#define MusicFlag_TASK_PRIO         16
#define MusicFlag_STACK_SIZE        128
TaskHandle_t MusicFlag_Task_handler;
StackType_t   MusicFlag_stack[MusicFlag_STACK_SIZE];
StaticTask_t MusicFlag_tcb;
void MusicFlag_task(void *pvParameters);
/* Music��������
 * ����: �������ȼ� ��ջ��С ������ ��������
 */
#define Music_TASK_PRIO         16
#define Music_STACK_SIZE        128
TaskHandle_t Music_Task_handler;
StackType_t   Music_stack[Music_STACK_SIZE];
StaticTask_t Music_tcb;
void Music_task(void *pvParameters);

//����С���ݶ���
#define Key_QUEUE_LENGTH    2                   /* ����֧�ֵ���Ϣ���� */
#define Key_QUEUE_ITEM_SIZE sizeof(uint8_t)     /* ������ÿ����Ϣ�Ĵ�С Ҫƥ����Ҫ��ӵ���������*/
uint8_t Key_QUEUE_BUF[Key_QUEUE_LENGTH];
StaticQueue_t Key_pxQueueBuffer;
QueueHandle_t key_Queue;                        //���
//��������ݶ���
#define big_data_QUEUE_LENGTH    2                   /* ����֧�ֵ���Ϣ���� */
#define big_data_QUEUE_ITEM_SIZE sizeof(char *)     /* ������ÿ����Ϣ�Ĵ�С  Ҫƥ����Ҫ��ӵ���������*/
uint8_t big_data_QUEUE_BUF[big_data_QUEUE_LENGTH];
StaticQueue_t big_data_pxQueueBuffer;
QueueHandle_t big_data_Queue;                   //���

char big_data_buff_A[100] = {"����һ�������飬��������1234 ABCDEF"};
char big_data_buff_B[100] = {"������������"};

extern TaskHandle_t  Start_task_handler;
void Start_Task(void * pvParameters)
{
    taskENTER_CRITICAL();//�����ٽ���������������ɺ󣬲ſ�ʼִ�����ȼ����� 

    key_Queue              =    xQueueCreateStatic(Key_QUEUE_LENGTH ,
                                                   Key_QUEUE_ITEM_SIZE , 
                                                   Key_QUEUE_BUF ,
                                                   &Key_pxQueueBuffer);
    if(key_Queue != NULL)
    {
        printf("key_Queue���д����ɹ�����\r\n");
    }
    else
    {
        printf("key_Queue���д���ʧ�ܣ���\r\n");
    }
    big_data_Queue              =  xQueueCreateStatic(big_data_QUEUE_LENGTH , 
                                                    big_data_QUEUE_ITEM_SIZE , 
                                                    big_data_QUEUE_BUF ,
                                                    &big_data_pxQueueBuffer);
    if(big_data_Queue != NULL)
    {
        printf("big_data_Queue���д����ɹ�����\r\n");
    }
    else
    {
        printf("big_data_Queue���д���ʧ�ܣ���\r\n");
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
    vTaskDelete(Start_task_handler);//ɾ����ʼ����ɾ��������NULL

    taskEXIT_CRITICAL();//�˳��ٽ���		
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
            printf("key_Queue���ж�ȡʧ�ܣ���\r\n");
        }
        else
        {
            printf("key_Queue���ж�ȡ�ɹ��������ݣ�%d\r\n", key);//����
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
        //����task2�������ȼ���task1�ߣ�����˵����Ҫ����ʱ��������������task1�������У���������ǵ�����xQueueReceive�������˺�����������ǿյģ���ô��ȡ���ˣ�
        //���������Ӿ���̬�Ƴ����ƶ�������̬�����task2Ҳ�ͽ�������̬�ˣ���˾���û�е�����ʱ������task2Ҳ���������̬
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

        // if(buf == "����һ�������飬��������1234 ABCDEF")
        // {
        //     LED1_SetMode(4);
        //     LED2_SetMode(4);
        // }��C������ֱ��ʹ�� == �Ƚ��ַ�����Ƚ�ָ���ַ�������ݣ����������ж�ʧЧ�ĸ���ԭ��
        // ʹ��strcmp�������ݱȽ�
        if(err != pdTRUE)
        {
            printf("big_data_Queue���ж�ȡʧ�ܣ���\r\n");
        }
        else
        {
            printf("big_data_Queue���ж�ȡ�ɹ��������ݣ�%s\r\n", buf);//����
        }
        if(strcmp(buf, "����һ�������飬��������1234 ABCDEF") == 0) //strcmp����<string.h>������������ͷ�ļ���
        {
            LED1_SetMode(4);
            LED2_SetMode(4);
            Motor_SetSpeed(90);
            printf("�ַ���ƥ��ɹ���LEDģʽ���л���\r\n");
        }
        else if(strcmp(buf, "������������") == 0) 
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
        atk_aht20_read_data(&temperature, &humidity);       /* ��ȡATH20���������� */
        printf("\r\n�¶�: %.2f��\r\n", temperature);            /* ����õ�ʪ��ֵ */
        printf("\r\nʪ��: %.2f%%\r\n", humidity);               /* ����õ��¶�ֵ */
        OLED_ClearArea(49,0,8,16);
        OLED_ClearArea(49,17,8,16);
        OLED_ShowFloatNum(49, 0, temperature, 2, 1, OLED_8X16);
        OLED_ShowChinese(92,0,"��");
        OLED_ShowFloatNum(49, 17, humidity, 2, 1, OLED_8X16);
        OLED_ShowString(92,17, "%RH", OLED_8X16);


        
        // ��ȡ��ǰ����Ķ�ջ��ˮλ��
        UBaseType_t highWaterMark_words = uxTaskGetStackHighWaterMark(NULL);
        UBaseType_t highWaterMark_bytes = highWaterMark_words * sizeof(StackType_t); // ת��Ϊ�ֽ�
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
        printf("\r\nLight=%d\r\n", Light);               /* ����õ��¶�ֵ */
        OLED_ShowNum(58, 33, Light, 2, OLED_8X16);
        vTaskDelay(1000);
    }
}
// //tsak1��������ʵ�����
void Key_task(void *pvParameters)
{ 
    uint8_t mykey = 0;
    BaseType_t err = 0;
    char * buf_A;
    char * buf_B;
    buf_A = big_data_buff_A;//��Ч��buf = &buff[0]
    buf_B = big_data_buff_B;
    while(1)
    {
        // printf("\r\nKey_task!\r\n");
        mykey = Key_GetNum();
        // printf("%d\r\n",mykey);
        if(mykey == KEY1_SingleClick || mykey == KEY1_LongCick || mykey == KEY1_DobleClick || mykey == KEY3_SingleClick || mykey == KEY3_LongCick)
        {
            err = xQueueSend(key_Queue, &mykey, portMAX_DELAY);//portMAX_DELAY�����ʱҲ��������
            if(err != pdTRUE)
            {
                printf("key_Queue���з���ʧ�ܣ���\r\n");
            }
            else
            {
                printf("key_Queue���з��ͳɹ�����\r\n");//���
            }
        }
        else if(mykey == KEY2_SingleClick)
        {
            err = xQueueSend(big_data_Queue, &buf_A, portMAX_DELAY);//portMAX_DELAY�����ʱҲ��������
            if(err != pdTRUE)
            {
                printf("big_data_Queue���з���ʧ�ܣ���\r\n");
            }
            else
            {
                printf("big_data_Queue���з��ͳɹ�����\r\n");//���
            }
        }
        else if(mykey == KEY2_DobleClick)
        {
            err = xQueueSend(big_data_Queue, &buf_B, portMAX_DELAY);//portMAX_DELAY�����ʱҲ��������
            if(err != pdTRUE)
            {
                printf("big_data_Queue���з���ʧ�ܣ���\r\n");
            }
            else
            {
                printf("big_data_Queue���з��ͳɹ�����\r\n");//���
            }
        }
        OLED_Update();
        vTaskDelay(100);//�����ʱ
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
            printf("key_Queue���ж�ȡʧ�ܣ���\r\n");
        }
        else
        {
            printf("key_Queue���ж�ȡ�ɹ��������ݣ�%d\r\n", key);//����
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
        vTaskDelay(100);//�����ʱ
    }
}
