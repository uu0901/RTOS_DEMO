#include "stm32f10x.h"                  // Device header
#include "key.h"                  // Device header
uint8_t Key_Num;

void Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);


    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;      //KEY1
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;      //KEY2
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(GPIOB, &GPIO_InitStructure);

     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;     //KEY3
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(GPIOA, &GPIO_InitStructure);
}

uint8_t Key_GetNum(void)
{
    /* 
    此代码不合理，如果中断发生在Temp = Key_Num; Key_Num = 0; 这两句之间，
    并且中断里置了键码标志位，那么中断退出后，回立刻执行key_Nm=0，
    这样再次调用此函数就是返回0了，也就是此次按键事件就会被忽略。
    所以要多加一个if判断，如果key_num不为0，再执行读后清0的操作 
    */
    //uint8_t Temp;
    //Temp = Key_Num;
    //Key_Num = 0;
    //return Temp;
    

    uint8_t Temp;
    if (Key_Num)
    {
        Temp = Key_Num;
        Key_Num = 0;

        return Temp;
    }
    return 0;
}

uint8_t Key_GetState(void)//获取按键状态
{
    if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) == 0) //ROCK1_KEY按下
    {
        return KEY1;
    }
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0) //ROCK2_KEY按下
    {
        return KEY2;
    }
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 0)//KEY_L按下
    {
        return KEY3;
    }
    return PRESS_NONE;//无按键按下
}


void Key_Tick(void)//按键扫描1ms调用一次
{
    //静态变量默认初值为0
    static uint8_t Count;                                    //计数值
    static uint8_t CurrentKeyVal, PreviousKeyVal;           //本次状态和上一次状态
    static uint16_t PressDownCnt;
    static uint16_t RePressDownCnt;
    static uint16_t PressUpCnt;
    static uint8_t step;  
    static uint8_t KEY1_FLAG, KEY2_FLAG, KEY3_FLAG;//单击标志位
    Count ++;
    if (Count >= 1)                                //20ms调用一次Key_GetState
    {
        Count = 0;

        PreviousKeyVal = CurrentKeyVal;                     //上次状态，也就是上一次的本次
        CurrentKeyVal = Key_GetState();                      //本次状态
    }
    if(step == 0)
    {
        PressDownCnt = 0; 
        RePressDownCnt = 0;
        PressUpCnt = 0;
        if (CurrentKeyVal !=0  && PreviousKeyVal == 0)      //本次为0 上次非0，说明有按键按下并松手  按下为1 松开为0
        {
            step = 1;
            if(CurrentKeyVal == KEY1) KEY1_FLAG = 1;
            if(CurrentKeyVal == KEY2) KEY2_FLAG = 1;
            if(CurrentKeyVal == KEY3) KEY3_FLAG = 1;
        }
    }
    else if (step == 1)
    {
        if(CurrentKeyVal != PRESS_NONE)//按下
        {
            PressDownCnt ++;
            if(PressDownCnt >= LongTime)//长按
            {
                if(CurrentKeyVal == KEY1)
                {
                    Key_Num = KEY1_LongCick;
                    KEY1_FLAG = 0;
                }
                else if(CurrentKeyVal == KEY2)
                {
                    Key_Num = KEY2_LongCick;
                    KEY2_FLAG = 0;
                }
                else if(CurrentKeyVal == KEY3)
                {
                    Key_Num = KEY3_LongCick;
                    KEY3_FLAG = 0;
                }
                step = 0;
            }
        }
        else //弹起
        {

            step = 2;
        }
    }  
    else if (step == 2)//弹起
    {
        PressUpCnt ++;
        if(PressUpCnt <= DoubleTime)
        {
            if(CurrentKeyVal != PRESS_NONE)//按下
            {
                RePressDownCnt ++;
            }
            if(RePressDownCnt > KeyDelay)//双击
            {
                if(CurrentKeyVal == KEY1)
                {
                    KEY1_FLAG = 0;
                    Key_Num = KEY1_DobleClick;
                }
                else if(CurrentKeyVal == KEY2)
                {
                    KEY2_FLAG = 0;
                    Key_Num = KEY2_DobleClick;
                }
                else if(CurrentKeyVal == KEY3)
                {
                    KEY3_FLAG = 0;
                    Key_Num = KEY3_DobleClick;
                }
                step = 0;
            }
        }
        else//弹起
        {
 
            if(PressDownCnt > KeyDelay)//单击
            {
                if(KEY1_FLAG == 1)
                {
                    KEY1_FLAG = 0;
                    Key_Num = KEY1_SingleClick;
                }
                else if(KEY2_FLAG == 1)
                {
                    KEY2_FLAG = 0;
                    Key_Num = KEY2_SingleClick;
                }
                else if(KEY3_FLAG == 1)
                {
                    KEY3_FLAG = 0;
                    Key_Num = KEY3_SingleClick;
                }
                step = 0;
            }
        }
    }
}
        

