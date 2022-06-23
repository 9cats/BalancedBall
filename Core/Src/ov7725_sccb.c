/*********************** HNIT 3103 Application Team **************************
 * 文 件 名 ：hnit_sccb.c
 * 描    述 ：SCCB协议实现
 * 实验平台 ：STM32F407开发板
 * 库 版 本 ：ST1.4.0
 * 时    间 ：2016.04.08
 * 作    者 ：3103创新团队 王昱霏
 * 修改记录 ：无
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
  SCCB_SDA(1); //数据线高电平
  SCCB_SCL(1); //在时钟线高的时候数据线由高至低
  delay_us(50);
  SCCB_SDA(0);
  delay_us(50);
  SCCB_SCL(0); //数据线恢复低电平，单操作函数必要
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
  for (j = 0; j < 8; j++) //循环8次发送数据
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
  SCCB_SDA_IN(); //设置SDA为输入
  delay_us(50);
  SCCB_SCL(1); //接收第九位,以判断是否发送成功
  delay_us(50);
  if (SCCB_READ_SDA)
    res = 1; // SDA=1发送失败，返回1
  else
    res = 0; // SDA=0发送成功，返回0
  SCCB_SCL(0);
  SCCB_SDA_OUT(); //设置SDA为输出
  return res;
}

uint8_t SCCB_RD_Byte(void)
{
  uint8_t temp = 0, j;
  SCCB_SDA_IN();          //设置SDA为输入
  for (j = 8; j > 0; j--) //循环8次接收数据
  {
    delay_us(50);
    SCCB_SCL(1);
    temp = temp << 1;
    if (SCCB_READ_SDA)
      temp++;
    delay_us(50);
    SCCB_SCL(0);
  }
  SCCB_SDA_OUT(); //设置SDA为输出
  return temp;
}

uint8_t SCCB_WR_Reg(uint8_t reg, uint8_t data)
{
  uint8_t res = 0;
  SCCB_Start(); //启动SCCB传输
  if (SCCB_WR_Byte(SCCB_ID))
    res = 1; //写器件ID
  delay_us(100);
  if (SCCB_WR_Byte(reg))
    res = 1; //写寄存器地址
  delay_us(100);
  if (SCCB_WR_Byte(data))
    res = 1; //写数据
  SCCB_Stop();
  return res;
}
uint8_t SCCB_RD_Reg(uint8_t reg)
{
  uint8_t val = 0;
  SCCB_Start();          //启动SCCB传输
  SCCB_WR_Byte(SCCB_ID); //写器件ID
  delay_us(100);
  SCCB_WR_Byte(reg); //写寄存器地址
  delay_us(100);
  SCCB_Stop();
  delay_us(100);
  //设置寄存器地址后，才是读
  SCCB_Start();
  SCCB_WR_Byte(SCCB_ID | 0X01); //发送读命令
  delay_us(100);
  val = SCCB_RD_Byte(); //读取数据
  SCCB_No_Ack();
  SCCB_Stop();
  return val;
}
