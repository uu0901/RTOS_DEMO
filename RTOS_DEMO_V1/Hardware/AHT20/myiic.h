
#ifndef __MYIIC_H
#define __MYIIC_H

#include "sys.h"


/******************************************************************************************/
/* ���� ���� */

#define IIC_SCL_GPIO_PORT               GPIOA
#define IIC_SCL_GPIO_PIN                GPIO_Pin_2     //GPIO_Pin_5
#define IIC_SCL_GPIO_CLK_ENABLE()       do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); }while(0)     /* PA��ʱ��ʹ��  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);	*/

#define IIC_SDA_GPIO_PORT               GPIOA
#define IIC_SDA_GPIO_PIN                GPIO_Pin_3
#define IIC_SDA_GPIO_CLK_ENABLE()       do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); }while(0)     /* PA��ʱ��ʹ�� */

/******************************************************************************************/
/* IO���� */
//GPIO_WriteBit(I2C_GPIO_PORT, I2C_SCL_GPIO_PIN, (BitAction)BitValue);
#define IIC_SCL(x)        do{ x ? \
                              GPIO_WriteBit(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN, Bit_SET): \
                              GPIO_WriteBit(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN, Bit_RESET); \
                          }while(0)         /* SCL */

#define IIC_SDA(x)        do{ x ? \
                                GPIO_WriteBit(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, Bit_SET) : \
                                GPIO_WriteBit(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, Bit_RESET); \
                          }while(0)         /* SDA */

#define IIC_READ_SDA     GPIO_ReadInputDataBit(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN)        /* ��ȡSDA */ //�˴�����Ҫ����whileѭ�� ���ﲻ�ܴ��ֺţ�������ᵼ���﷨����


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

