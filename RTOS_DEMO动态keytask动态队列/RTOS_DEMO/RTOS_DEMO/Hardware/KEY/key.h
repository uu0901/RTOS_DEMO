#ifndef __KEY_H
#define __KEY_H
#include "sys.h"            //解决头文件uint8_t红叉
#define KEY1                  1
#define KEY2                  2
#define KEY3                  3
#define PRESS_NONE           0

#define LongTime        1500
#define DoubleTime      500
#define KeyDelay        30

#define Key_None                  99

#define KEY1_SingleClick     1
#define KEY1_DobleClick      2
#define KEY1_LongCick        3

#define KEY2_SingleClick     4
#define KEY2_DobleClick      5
#define KEY2_LongCick        6

#define KEY3_SingleClick         7
#define KEY3_DobleClick          8
#define KEY3_LongCick            9


void Key_Init(void);
uint8_t Key_GetNum(void);
void Key_Tick(void);

#endif
