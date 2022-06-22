#ifndef OV7725_SCCB_H
#define OV7725_SCCB_H

#include "main.h"


#define SCL_H         HAL_GPIO_WritePin(OV7725_SCL_GPIO_Port, OV7725_SCL_Pin, GPIO_PIN_SET)
#define SCL_L         HAL_GPIO_WritePin(OV7725_SCL_GPIO_Port, OV7725_SCL_Pin, GPIO_PIN_RESET)

#define SDA_H         HAL_GPIO_WritePin(OV7725_SDA_GPIO_Port, OV7725_SDA_Pin, GPIO_PIN_SET)
#define SDA_L         HAL_GPIO_WritePin(OV7725_SDA_GPIO_Port, OV7725_SDA_Pin, GPIO_PIN_RESET)

#define SDA_SET_IN    sccb_sda_set_input()
#define SDA_SET_OUT   sccb_sda_set_output()

#define SDA_read      HAL_GPIO_ReadPin(OV7725_SDA_GPIO_Port, OV7725_SDA_Pin)

#define ADDR_OV7725   0x42

void sccb_gpio_config(void);
int sccb_write_byte( uint16_t WriteAddress , uint8_t SendByte);
int sccb_read_byte(uint8_t* pBuffer,   uint16_t length,   uint8_t ReadAddress);

#endif
