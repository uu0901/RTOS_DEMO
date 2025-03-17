#include "myiic.h"
#include "delay.h"

/**
 * @brief       ��ʼ��IIC
 * @param       ��
 * @retval      ��
 */
void iic_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    IIC_SCL_GPIO_CLK_ENABLE();                           /* SCL����ʱ��ʹ�� */
    IIC_SDA_GPIO_CLK_ENABLE();                           /* SDA����ʱ��ʹ�� */

    GPIO_InitStructure.GPIO_Pin = IIC_SCL_GPIO_PIN;             /* SCL���� */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         /* ������� */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       /* ���� */
    GPIO_Init(IIC_SCL_GPIO_PORT, &GPIO_InitStructure);

    /* SDA��������Ϊ��©���,�ⲿ������, �����Ͳ���������IO������ */
    GPIO_InitStructure.GPIO_Pin = IIC_SDA_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;         /* ��©��� */
    GPIO_Init(IIC_SDA_GPIO_PORT, &GPIO_InitStructure); /* SDA */

    iic_stop();                                          /* ֹͣ�����������豸 */
}

/**
 * @brief       IIC��ʱ����,���ڿ���IIC��д�ٶ�
 * @param       ��
 * @retval      ��
 */
static void iic_delay(void)
{
    Delay_us(2);    /* 2us����ʱ */
}

/**
 * @brief       ����IIC��ʼ�ź�
 * @param       ��
 * @retval      ��
 */
void iic_start(void)
{
    IIC_SDA(1);
    IIC_SCL(1);
    iic_delay();
    IIC_SDA(0);     /* START�ź�: ��SCLΪ��ʱ, SDA�Ӹ߱�ɵ�, ��ʾ��ʼ�ź� */
    iic_delay();
    IIC_SCL(0);     /* ǯסI2C���ߣ�׼�����ͻ�������� */
    iic_delay();
}

/**
 * @brief       ����IICֹͣ�ź�
 * @param       ��
 * @retval      ��
 */
void iic_stop(void)
{
    IIC_SDA(0);     /* STOP�ź�: ��SCLΪ��ʱ, SDA�ӵͱ�ɸ�, ��ʾֹͣ�ź� */
    iic_delay();
    IIC_SCL(1);
    iic_delay();
    IIC_SDA(1);     /* ����I2C���߽����ź� */
    iic_delay();
}

/**
 * @brief       �ȴ�Ӧ���źŵ���
 * @param       ��
 * @retval      1������Ӧ��ʧ��
 *              0������Ӧ��ɹ�
 */
uint8_t iic_wait_ack(void)
{
    uint8_t waittime = 0;
    uint8_t rack = 0;

    IIC_SDA(1);     /* �����ͷ�SDA��(��ʱ�ⲿ������������SDA��) */
    iic_delay();
    IIC_SCL(1);     /* SCL=1, ��ʱ�ӻ����Է���ACK */
    iic_delay();

    while (IIC_READ_SDA)    /* �ȴ�Ӧ�� */
    {
        waittime++;

        if (waittime > 250)
        {
            iic_stop();
            rack = 1;
            break;
        }
    }

    IIC_SCL(0);     /* SCL=0, ����ACK��� */
    iic_delay();
    return rack;
}

/**
 * @brief       ����ACKӦ��
 * @param       ��
 * @retval      ��
 */
 /* ACK�����ã������ڽ���ģʽ�£���Ҫ��ȷ���ߴӻ�"����ȷ��������"��
   Э��Ҫ����SCL�ߵ�ƽ�ڼ䣬SDA���뱻�������ͣ���������ӻ�ʵ�֣���
   
    SCL: _/������\__...
	SDA: ��\____/��...  �� �������ͺ������ͷ�
 */
void iic_ack(void)
{
    IIC_SDA(0);     /* SCL 0 -> 1  ʱ SDA = 0,��ʾӦ�� */
    iic_delay();
    IIC_SCL(1);     /* ����һ��ʱ�� */
    iic_delay();
    IIC_SCL(0);
    iic_delay();
    IIC_SDA(1);     /* �����ͷ�SDA�� */  /* �����ͷ�SDA�����������ͷ�SDA�ߣ����߽����ڱ����ͣ����´ӻ��޷��ں��������п���SDA�� */
    iic_delay();
}

/**
 * @brief       ������ACKӦ��
 * @param       ��
 * @retval      ��
 */
 	/*  NACK�����ã������ڽ���ģʽ�£�ͨ������SDA�ߵ�ƽ���ߴӻ�"δ��ȷ�������ݻ���ֹͨ��"��
       Э��Ҫ����SCL�ߵ�ƽ�ڼ䣬SDA���뱣�ָߵ�ƽ�� 
	   
	    SCL: _/������\__...
		SDA: ����������������...  �� SDA���ָߵ�ƽ�������Ԥ
    */
void iic_nack(void)
{
    IIC_SDA(1);     /* SCL 0 -> 1  ʱ SDA = 1,��ʾ��Ӧ�� */
    iic_delay();
    IIC_SCL(1);     /* ����һ��ʱ�� */
    iic_delay();
    IIC_SCL(0);
    iic_delay();
}

/**
 * @brief       IIC����һ���ֽ�
 * @param       data: Ҫ���͵�����
 * @retval      ��
 */
void iic_send_byte(uint8_t data)
{
    uint8_t t;
    
    for (t = 0; t < 8; t++)
    {
        IIC_SDA((data & 0x80) >> 7);    /* ��λ�ȷ��� */
        iic_delay();
        IIC_SCL(1);
        iic_delay();
        IIC_SCL(0);
        data <<= 1;     /* ����1λ,������һ�η��� */
    }
    IIC_SDA(1);         /* �������, �����ͷ�SDA�� */  
	/* IIC Э��Ҫ�������ڷ��������ݺ��ͷ� SDA���Ա�ӻ�Ӧ�𡣵ڶ��δ������ں������� SDA �ͷ��߼���*/
}

/**
 * @brief       IIC��ȡһ���ֽ�
 * @param       ack:  ack=1ʱ������ack; ack=0ʱ������nack
 * @retval      ���յ�������
 */
uint8_t iic_read_byte(uint8_t ack)
{
    uint8_t i, receive = 0;

    for (i = 0; i < 8; i++ )    /* ����1���ֽ����� */
    {
        receive <<= 1;  /* ��λ�����,�������յ�������λҪ���� */
        IIC_SCL(1);
        iic_delay();

        if (IIC_READ_SDA)
        {
            receive++;
        }
        
        IIC_SCL(0);
        iic_delay();
    }

    if (!ack)
    {
        iic_nack();     /* ����nACK */
    }
    else
    {
        iic_ack();      /* ����ACK */
    }

    return receive;
}









