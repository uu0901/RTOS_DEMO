
#ifndef __MYIIC_H
#define __MYIIC_H

#include "sys.h"


/******************************************************************************************/
/* 引脚 定义 */

#define IIC_SCL_GPIO_PORT               GPIOA
#define IIC_SCL_GPIO_PIN                GPIO_Pin_2     //GPIO_Pin_5
#define IIC_SCL_GPIO_CLK_ENABLE()       do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); }while(0)     /* PA口时钟使能  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);	*/

#define IIC_SDA_GPIO_PORT               GPIOA
#define IIC_SDA_GPIO_PIN                GPIO_Pin_3
#define IIC_SDA_GPIO_CLK_ENABLE()       do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); }while(0)     /* PA口时钟使能 */

/******************************************************************************************/
/* IO操作 */
//GPIO_WriteBit(I2C_GPIO_PORT, I2C_SCL_GPIO_PIN, (BitAction)BitValue);
#define IIC_SCL(x)        do{ x ? \
                              GPIO_WriteBit(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN, Bit_SET): \
                              GPIO_WriteBit(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN, Bit_RESET); \
                          }while(0)         /* SCL */

#define IIC_SDA(x)        do{ x ? \
                                GPIO_WriteBit(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, Bit_SET) : \
                                GPIO_WriteBit(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, Bit_RESET); \
                          }while(0)         /* SDA */

#define IIC_READ_SDA     GPIO_ReadInputDataBit(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN)        /* 读取SDA */ //此处由于要填入while循环 这里不能带分号，带了这会导致语法错误，


/******************************************************************************************/
                          
void iic_init(void);                        /* 初始化IIC的IO口 */
void iic_start(void);                       /* 发送IIC开始信号 */
void iic_stop(void);                        /* 发送IIC停止信号 */
void iic_ack(void);                         /* IIC发送ACK信号 */
void iic_nack(void);                        /* IIC不发送ACK信号 */
uint8_t iic_wait_ack(void);                 /* IIC等待ACK信号 */
void iic_send_byte(uint8_t txd);            /* IIC发送一个字节 */
uint8_t iic_read_byte(unsigned char ack);   /* IIC读取一个字节 */

#endif

