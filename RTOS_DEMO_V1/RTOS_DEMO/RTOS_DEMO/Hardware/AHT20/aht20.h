#ifndef __ATK_AHT20_H
#define __ATK_AHT20_H


#include "sys.h"
#include "myiic.h"
#include "delay.h"


/**************************************************************************************************/

#define ATK_AHT20_IIC_ADDR          0x70    /* AHT20上电默认IIC通讯地址 */

/* AHT20指令 */
#define INIT                        0xBE    /* 初始化指令 */
#define START_TEST                  0xAC    /* 开始测量指令 */

/**************************************************************************************************/

uint8_t Aht20_Init(void);                                                   /* AHT20传感器初始化 */
uint8_t atk_aht20_read_status(void);                                            /* 读取AHT20状态 */
uint8_t atk_aht20_check(void);                                                  /* 检测AHT20是否就绪 */
void atk_aht20_read_data(float *temp, float *humi) ;                            /* 读取AHT20的温度和湿度数据 */
uint8_t atk_aht20_write_nbytes(uint8_t reg_addr, uint8_t *data, uint8_t len);   /* 写N个字节数据 */
uint8_t atk_aht20_read_nbytes(uint8_t *data, uint8_t len);                      /* 读N个字节数据 */

#endif





