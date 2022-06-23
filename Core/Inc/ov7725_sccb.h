#ifndef OV7725_SCCB_H
#define OV7725_SCCB_H

#include "main.h"


//IO操作函数
#define SCCB_SCL(n)   		(n?HAL_GPIO_WritePin(OV7725_SCL_GPIO_Port, OV7725_SCL_Pin, GPIO_PIN_SET):HAL_GPIO_WritePin(OV7725_SCL_GPIO_Port, OV7725_SCL_Pin, GPIO_PIN_RESET))//SCL
#define SCCB_SDA(n)    		(n?HAL_GPIO_WritePin(OV7725_SDA_GPIO_Port, OV7725_SDA_Pin, GPIO_PIN_SET):HAL_GPIO_WritePin(OV7725_SDA_GPIO_Port, OV7725_SDA_Pin, GPIO_PIN_RESET))//SDA

#define SCCB_READ_SDA       HAL_GPIO_ReadPin(OV7725_SDA_GPIO_Port, OV7725_SDA_Pin)//输入SDA
#define SCCB_ID   			0X42  			//OV7670的ID

void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
uint8_t SCCB_WR_Byte(uint8_t dat);
uint8_t SCCB_RD_Byte(void);
uint8_t SCCB_WR_Reg(uint8_t reg,uint8_t data);
uint8_t SCCB_RD_Reg(uint8_t reg);

#endif
