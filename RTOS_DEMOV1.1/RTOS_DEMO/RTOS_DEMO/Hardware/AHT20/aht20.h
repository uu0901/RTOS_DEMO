#ifndef __ATK_AHT20_H
#define __ATK_AHT20_H


#include "sys.h"
#include "myiic.h"
#include "delay.h"


/**************************************************************************************************/

#define ATK_AHT20_IIC_ADDR          0x70    /* AHT20�ϵ�Ĭ��IICͨѶ��ַ */

/* AHT20ָ�� */
#define INIT                        0xBE    /* ��ʼ��ָ�� */
#define START_TEST                  0xAC    /* ��ʼ����ָ�� */

/**************************************************************************************************/

uint8_t Aht20_Init(void);                                                   /* AHT20��������ʼ�� */
uint8_t atk_aht20_read_status(void);                                            /* ��ȡAHT20״̬ */
uint8_t atk_aht20_check(void);                                                  /* ���AHT20�Ƿ���� */
void atk_aht20_read_data(float *temp, float *humi) ;                            /* ��ȡAHT20���¶Ⱥ�ʪ������ */
uint8_t atk_aht20_write_nbytes(uint8_t reg_addr, uint8_t *data, uint8_t len);   /* дN���ֽ����� */
uint8_t atk_aht20_read_nbytes(uint8_t *data, uint8_t len);                      /* ��N���ֽ����� */

#endif





