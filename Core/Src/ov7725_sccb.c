/*********************** HNIT 3103 Application Team **************************
 * �� �� �� ��hnit_sccb.c
 * ��    �� ��SCCBЭ��ʵ��
 * ʵ��ƽ̨ ��STM32F407������
 * �� �� �� ��ST1.4.0
 * ʱ    �� ��2016.04.08
 * ��    �� ��3103�����Ŷ� ������
 * �޸ļ�¼ ����
******************************************************************************/
#include "ov7725_sccb.h"

#define DEV_ADR  ADDR_OV7725 			 /*�豸��ַ����*/

/**
 * @brief  sda ����Ϊ����
 * @param  ��
 * @retval ��
 */
static void sccb_sda_set_input(void) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = OV7725_SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(OV7725_SDA_GPIO_Port, &GPIO_InitStruct);
}

/**
 * @brief  sda ����Ϊ���
 * @param  ��
 * @retval ��
 */
static void sccb_sda_set_output(void) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = OV7725_SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(OV7725_SDA_GPIO_Port, &GPIO_InitStruct);
}

/**
  * @brief  SCCBЭ��ר����ʱ
  * @param  ��
  * @retval ��
  */
static void sccb_delay(void)
{
    uint16_t i = 800;
    while(i)
    {
        i--;
    }
}

/**
  * @brief  SCCB��ʼ�ź�
  * @param  ��
  * @retval ��
  */
static int sccb_start(void)
{
	SDA_H;

	SDA_SET_OUT;
    SDA_H;
    SCL_H;

    SDA_SET_IN;
    sccb_delay();
    if(!SDA_read)
        return DISABLE;	/* SDA��Ϊ�͵�ƽ������æ,�˳� */

    SDA_SET_OUT;
    SDA_L;

    SDA_SET_IN;
    sccb_delay();
    if(SDA_read)
        return DISABLE;	/* SDA��Ϊ�ߵ�ƽ�����߳���,�˳� */

	SDA_SET_OUT;
    SDA_L;
    sccb_delay();

    return ENABLE;
}

/**
  * @brief  SCCBֹͣ�ź�
  * @param  ��
  * @retval ��
  */
static void sccb_stop(void)
{
	SDA_SET_OUT;
    SCL_L;
    sccb_delay();
    SDA_L;
    sccb_delay();
    SCL_H;
    sccb_delay();
    SDA_H;
    sccb_delay();
}

/**
  * @brief  SCCBӦ��
  * @param  ��
  * @retval ��
  */
static void sccb_ack(void)
{
	SDA_SET_OUT;
    SCL_L;
    sccb_delay();
    SDA_L;
    sccb_delay();
    SCL_H;
    sccb_delay();
    SCL_L;
    sccb_delay();
}

/**
  * @brief  SCCB ��Ӧ��
  * @param  ��
  * @retval ��
  */
static void sccb_no_ack(void)
{
	SDA_SET_OUT;
    SCL_L;
    sccb_delay();
    SDA_H;
    sccb_delay();
    SCL_H;
    sccb_delay();
    SCL_L;
    sccb_delay();
}

/**
  * @brief  SCCB �ȴ�Ӧ��
  * @param  ��
  * @retval 1����ACK
  *         0����ACK
  */
static int sccb_wait_ack(void)
{
	SDA_SET_OUT;
    SCL_L;
    sccb_delay();
    SDA_H;
    sccb_delay();
    SCL_H;
    sccb_delay();
	SDA_SET_IN;

	SDA_SET_IN;
    if(SDA_read)
    {
        SCL_L;
        return DISABLE;
    }
    SCL_L;
    return ENABLE;
}

/**
  * @brief  ���ݴӸ�λ����λ
  * @param  SendByte ���͵�����
  * @retval ��
  */
static void sccb_send_byte(uint8_t SendByte)
{
    uint8_t i=8;
    SDA_SET_OUT;
    while(i--)
    {
        SCL_L;
        sccb_delay();
      if(SendByte&0x80)
        SDA_H;
      else
        SDA_L;
        SendByte<<=1;
        sccb_delay();
		SCL_H;
        sccb_delay();
    }
    SCL_L;
}

/**
  * @brief  ���ݴӸ�λ����λ
  * @param  ��
  * @retval SCCB���߷��ص�����
  */
static int sccb_receive_byte(void)
{
    uint8_t i=8;
    uint8_t ReceiveByte=0;

    SDA_SET_OUT;
    SDA_H;
    while(i--)
    {
      ReceiveByte<<=1;
      SCL_L;
      sccb_delay();
	  SCL_H;
      sccb_delay();

      SDA_SET_IN;
      if(SDA_read)
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L;
    return ReceiveByte;
}

/**
  * @brief  дһ�ֽ�����
  * @param  WriteAddress ��д���ַ
  * @param  SendByte     ��д������
  * @retval ��
  */
int sccb_write_byte( uint16_t WriteAddress , uint8_t SendByte )
{
    if(!sccb_start())
    {
        return DISABLE;
    }
    sccb_send_byte( DEV_ADR );                    /* ������ַ */
    if( !sccb_wait_ack() )
    {
        sccb_stop();
        return DISABLE;
    }

    sccb_send_byte((uint8_t)(WriteAddress & 0x00FF));   /* ���õ���ʼ��ַ */
    sccb_wait_ack();
    sccb_send_byte(SendByte);
    sccb_wait_ack();
    sccb_stop();
    return ENABLE;
}

/**
  * @brief  ��ȡһ������
  * @param  pBuffer     ��Ŷ�������
  * @param  ReadAddress ��������ַ
  * @retval 1���ɹ�����
  *         2��ʧ��
  */
int sccb_read_byte(uint8_t* pBuffer, uint16_t length, uint8_t ReadAddress)
{
    if(!sccb_start())
    {
        return DISABLE;
    }
    sccb_send_byte( DEV_ADR );         /* ������ַ */
    if( !sccb_wait_ack() )
    {
        sccb_stop();
        return DISABLE;
    }
    sccb_send_byte( ReadAddress );     /* ���õ���ʼ��ַ */
    sccb_wait_ack();
    sccb_stop();

    if(!sccb_start())
    {
        return DISABLE;
    }
    sccb_send_byte( DEV_ADR + 1 );     /* ������ַ */
    if(!sccb_wait_ack())
    {
        sccb_stop();
        return DISABLE;
    }
    while(length)
    {
        *pBuffer = sccb_receive_byte();
        if(length == 1)
        {
            sccb_no_ack();
        }
        else
        {
            sccb_ack();
        }
        pBuffer++;
        length--;
    }
    sccb_stop();
    return ENABLE;
}
/*********************************************END OF FILE**********************/
