/**
 ****************************************************************************************************
 * @file        myiic.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2024-11-01
 * @brief       IIC ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� M48Z-M3��Сϵͳ��STM32F103��
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */
 
#ifndef __MYIIC_H
#define __MYIIC_H

#include "sys.h"


/******************************************************************************************/
/* ���� ���� */

#define IIC_SCL_GPIO_PORT               GPIOB
#define IIC_SCL_GPIO_PIN                GPIO_Pin_13
#define IIC_SCL_GPIO_CLK_ENABLE()       do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);}while(0)     /* PA��ʱ��ʹ�� */

#define IIC_SDA_GPIO_PORT               GPIOB
#define IIC_SDA_GPIO_PIN                GPIO_Pin_12
#define IIC_SDA_GPIO_CLK_ENABLE()       do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);}while(0)     /* PA��ʱ��ʹ�� */
/******************************************************************************************/
/* IO���� */

#define IIC_SCL(x)        do{ x ? \
                              GPIO_WriteBit(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN, Bit_SET) : \
                              GPIO_WriteBit(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN, Bit_RESET); \
                          }while(0)         /* SCL */

#define IIC_SDA(x)        do{ x ? \
                              GPIO_WriteBit(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, Bit_SET) : \
                              GPIO_WriteBit(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, Bit_RESET); \
                          }while(0)         /* SDA */

#define IIC_READ_SDA     GPIO_ReadInputDataBit(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN)          /* ��ȡSDA */


/******************************************************************************************/
                          
void iic_init(void);                        /* ��ʼ��IIC��IO�� */
void iic_start(void);                       /* ����IIC��ʼ�ź� */
void iic_stop(void);                        /* ����IICֹͣ�ź� */
void iic_ack(void);                         /* IIC����ACK�ź� */
void iic_nack(void);                        /* IIC������ACK�ź� */
uint8_t iic_wait_ack(void);                 /* IIC�ȴ�ACK�ź� */
void iic_send_byte(uint8_t txd);            /* IIC����һ���ֽ� */
uint8_t iic_read_byte(unsigned char ack);   /* IIC��ȡһ���ֽ� */

#endif

