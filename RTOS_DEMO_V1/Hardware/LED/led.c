#include "stm32f10x.h"                  // Device header
#include "led.h" 
uint8_t LED1_Mode;
uint8_t LED2_Mode;

uint16_t LED1_Count;                //LED1?????
uint16_t LED2_Count;

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);


    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_SetBits(GPIOB, GPIO_Pin_5);

}

void LED1_SetMode(uint8_t Mode)
{
    if (Mode != LED1_Mode)       //???????if???????????????????????????????????????????????????????count??led????????????????��?????????????????????
    {
        LED1_Mode = Mode;
        LED1_Count = 0;          //????��?LED1Mode??cout???????0????????????????????????
    }
}

void LED2_SetMode(uint8_t Mode)
{
    if (Mode != LED2_Mode)      //???????if???????????????????????????????????????????????????????count??led????????????????��?????????????????????
    {
        LED2_Mode = Mode;
        LED2_Count = 0;         //????��?LED2Mode??cout???????0????????????????????????
    }
}

void LED1_ON(void)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}

void LED1_OFF(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_5);
}

void LED2_ON(void)
{
    GPIO_ResetBits(GPIOE, GPIO_Pin_3);
}

void LED2_OFF(void)
{
    GPIO_SetBits(GPIOE, GPIO_Pin_3);
}



void LED_Tick(void)//led????????1ms???????
{
    if (LED1_Mode == 0)                 //mode=0??LED1
    {
        LED1_OFF();
    }
    else if (LED1_Mode == 1)            //mode=1??LED1
    {
        LED1_ON();
    }
    else if (LED1_Mode == 2)            //mode=2????LED1
    {
        LED1_Count ++;
        LED1_Count %= 1000;             //if(LED1_Count > 999)  LED1_Count = 0;     count<1000????????????count=1000????=0           =1000????0     ???0~999
        //LED1?????1ms*1000=1000ms
        if (LED1_Count < 500)           //0~499     [(499-0)+1]*1ms=500ms
        {
            LED1_ON();
        }
        else                              //500~999  [(999-500)+1]*1ms=500ms 
        {
            LED1_OFF();
        }
    }
    else if (LED1_Mode == 3)            //mode=3????LED1
    {
        LED1_Count ++;
        LED1_Count %= 100;              //????????100??0     =100????0     ???0~99
        //LED1?????1ms*100=100ms
        if (LED1_Count < 50)            //0~49     [(49-0)+1]*1ms=50ms
        {
            LED1_ON();
        }
        else                             //50~99    [(99-50)+1]*1ms=50ms
        {
            LED1_OFF();
        }
    }
    else if (LED1_Mode == 4)            //mode=4????LED1
    {
        LED1_Count ++;
        LED1_Count %= 1000;            //????????1000??0     =1000????0     ???0~999
        //LED1?????1ms*1000=1000ms
        if (LED1_Count < 100)           //0~99     [(99-0)+1]*1ms=100ms
        {
            LED1_ON();
        }
        else                             //100~999  [(999-100)+1]*1ms=900ms
        {
            LED1_OFF();
        }
    }

    //LED2???LED1????
    if (LED2_Mode == 0)
    {
        LED2_OFF();
    }
    else if (LED2_Mode == 1)
    {
        LED2_ON();
    }
    else if (LED2_Mode == 2)
    {
        LED2_Count ++;
        LED2_Count %= 1000;

        if (LED2_Count < 500)
        {
            LED2_ON();
        }
        else
        {
            LED2_OFF();
        }
    }
    else if (LED2_Mode == 3)
    {
        LED2_Count ++;
        LED2_Count %= 100;

        if (LED2_Count < 50)
        {
            LED2_ON();
        }
        else
        {
            LED2_OFF();
        }
    }
    else if (LED2_Mode == 4)
    {
        LED2_Count ++;
        LED2_Count %= 1000;

        if (LED2_Count < 100)
        {
            LED2_ON();
        }
        else
        {
            LED2_OFF();
        }
    }
}
