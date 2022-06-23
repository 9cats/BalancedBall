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
#include "delay.h"

void SCCB_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = OV7725_SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(OV7725_SDA_GPIO_Port, &GPIO_InitStruct);
}

void SCCB_SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = OV7725_SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
//	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(OV7725_SDA_GPIO_Port, &GPIO_InitStruct);
}

void SCCB_Init(void)
{
}

void SCCB_Start(void)
{
  SCCB_SDA(1); //�����߸ߵ�ƽ
  SCCB_SCL(1); //��ʱ���߸ߵ�ʱ���������ɸ�����
  delay_us(50);
  SCCB_SDA(0);
  delay_us(50);
  SCCB_SCL(0); //�����߻ָ��͵�ƽ��������������Ҫ
}

void SCCB_Stop(void)
{
  SCCB_SDA(0);
  delay_us(50);
  SCCB_SCL(1);
  delay_us(50);
  SCCB_SDA(1);
  delay_us(50);
}

void SCCB_No_Ack(void)
{
  delay_us(50);
  SCCB_SDA(1);
  SCCB_SCL(1);
  delay_us(50);
  SCCB_SCL(0);
  delay_us(50);
  SCCB_SDA(0);
  delay_us(50);
}

uint8_t SCCB_WR_Byte(uint8_t dat)
{
  uint8_t j, res;
  for (j = 0; j < 8; j++) //ѭ��8�η�������
  {
    if (dat & 0x80)
      SCCB_SDA(1);
    else
      SCCB_SDA(0);
    dat <<= 1;
    delay_us(50);
    SCCB_SCL(1);
    delay_us(50);
    SCCB_SCL(0);
  }
  SCCB_SDA_IN(); //����SDAΪ����
  delay_us(50);
  SCCB_SCL(1); //���յھ�λ,���ж��Ƿ��ͳɹ�
  delay_us(50);
  if (SCCB_READ_SDA)
    res = 1; // SDA=1����ʧ�ܣ�����1
  else
    res = 0; // SDA=0���ͳɹ�������0
  SCCB_SCL(0);
  SCCB_SDA_OUT(); //����SDAΪ���
  return res;
}

uint8_t SCCB_RD_Byte(void)
{
  uint8_t temp = 0, j;
  SCCB_SDA_IN();          //����SDAΪ����
  for (j = 8; j > 0; j--) //ѭ��8�ν�������
  {
    delay_us(50);
    SCCB_SCL(1);
    temp = temp << 1;
    if (SCCB_READ_SDA)
      temp++;
    delay_us(50);
    SCCB_SCL(0);
  }
  SCCB_SDA_OUT(); //����SDAΪ���
  return temp;
}

uint8_t SCCB_WR_Reg(uint8_t reg, uint8_t data)
{
  uint8_t res = 0;
  SCCB_Start(); //����SCCB����
  if (SCCB_WR_Byte(SCCB_ID))
    res = 1; //д����ID
  delay_us(100);
  if (SCCB_WR_Byte(reg))
    res = 1; //д�Ĵ�����ַ
  delay_us(100);
  if (SCCB_WR_Byte(data))
    res = 1; //д����
  SCCB_Stop();
  return res;
}
uint8_t SCCB_RD_Reg(uint8_t reg)
{
  uint8_t val = 0;
  SCCB_Start();          //����SCCB����
  SCCB_WR_Byte(SCCB_ID); //д����ID
  delay_us(100);
  SCCB_WR_Byte(reg); //д�Ĵ�����ַ
  delay_us(100);
  SCCB_Stop();
  delay_us(100);
  //���üĴ�����ַ�󣬲��Ƕ�
  SCCB_Start();
  SCCB_WR_Byte(SCCB_ID | 0X01); //���Ͷ�����
  delay_us(100);
  val = SCCB_RD_Byte(); //��ȡ����
  SCCB_No_Ack();
  SCCB_Stop();
  return val;
}
