
#include "ov7725_sccb.h"
#include "ov7725.h"
#include "delay.h"


typedef struct Reg
{
	uint8_t Address;
	uint8_t Value;
}Reg_Info;

/* �Ĵ����������� */
Reg_Info Sensor_Config[] =
{
        {0x11,0x00},//CLKRC
        {0x12,0x46},//COM7
        {0x17,0x3f},//HSTART
        {0x18,0x50},//HSIZE
        {0x19,0x03},//VSTRT
        {0x1a,0x78},//VSIZE
        {0x32,0x00},//HREF
        {0x29,0xa0},//HOutSize
        {0x2c,0xf0},//VOutSize
        {0x2a,0x00},//EXHCH

        {0x0c,0xd0},//COM3
        {0x42,0x7f},//TGT_B
        {0x4d,0x00},//FixGain 09
        {0x63,0xf0},//AWB_Ctrl0 e0
        {0x64,0xff},//DSP_Ctrl1 ff
        {0x65,0x20},//DSP_Ctrl2 20
        {0x66,0x00},//DSP_Ctrl3 00
        {0x67,0x00},//DSP_Ctrl4 00

        {0x13,0xff},//COM8  f0
        {0x0d,0xf0},//COM4  41 82
        {0x0f,0x01},//COM6  c5
        {0x14,0x06},//COM9  21
        {0x22,0xaf},//BDBase 7f 99 af ff
        {0x23,0x01},//BDMStep 03
        {0x24,0x75},//AEW 34
        {0x25,0x63},//AEB 3c
        {0x26,0xd1},//VPT a1
        {0x2b,0xff},//EXHCL 00
        {0x6b,0xaa},//AWBCtrl3
        {0x13,0xff},//COM8
        {0x69,0x50},//AWBCtrl1 5d

        {0x90,0x0a},//EDGE1
        {0x91,0x01},//DNSOff
        {0x92,0x01},//EDGE2
        {0x93,0x01},//EDGE3
        {0x94,0x2c},//MTX1 5f
        {0x95,0x24},//MTX2 53
        {0x96,0x08},//MTX3 11
        {0x97,0x14},//MTX4 1a
        {0x98,0x24},//MTX5 3d
        {0x99,0x38},//MTX6 5a
        {0x9a,0x9e},//MTX_Ctrl 1e

        {0x9b,0x00},//BRIGHT 80
        {0x9c,0x20},//CNST 25 30
        {0xa7,0x40},//USAT 65
        {0xa8,0x40},//VSAT 65
        {0x9e,0x11},//UVADJ0 81
        {0xa6,0x06},//SDE 06  �ڰ�
        {0x7e,0x0c},//GAM1
        {0x7f,0x16},//GAM2
        {0x80,0x2a},//GAM3
        {0x81,0x4e},//GAM4
        {0x82,0x61},//GAM5
        {0x83,0x6f},//GAM6
        {0x84,0x7b},//GAM7
        {0x85,0x86},//GAM8
        {0x86,0x8e},//GAM9
        {0x87,0x97},//GAM10
        {0x88,0xa4},//GAM11
        {0x89,0xaf},//GAM12
        {0x8a,0xc5},//GAM13
        {0x8b,0xd7},//GAM14
        {0x8c,0xe8},//GAM15
        {0x8d,0x20},//SLOP
        {0xa9,0x80},//HUECOS
        {0xaa,0x80},//HUESIN

        {0xac,0xff},//DSPAuto
        {0x33,0x00},//DM_LNL   40
        {0x22,0x99},//BDBase
        {0x23,0x03},//BDMStep
        {0x4a,0x00},//LC_RADI
        {0x49,0x13},//LC_COEF
        {0x47,0x08},//LC_XC
        {0x4b,0x14},//LC_COEFB
        {0x4c,0x17},//LC_COEFR
        {0x46,0x05},//LC_CTR
};



uint8_t OV7725_REG_NUM = sizeof(Sensor_Config)/sizeof(Sensor_Config[0]);

void OV7725_Reset(void)
{
	HAL_GPIO_WritePin(OV7725_RST_GPIO_Port, OV7725_RST_Pin, GPIO_PIN_RESET);
	delay_ms(10);
	HAL_GPIO_WritePin(OV7725_RST_GPIO_Port, OV7725_RST_Pin, GPIO_PIN_SET);
	delay_ms(10);
}

ErrorStatus OV7725_Init(void)
{
	uint16_t i = 0;
	uint8_t Sensor_IDCode = 0;

	OV7725_Reset();
	HAL_Delay(100);
	//DEBUG("ov7725 Register Config Start......");

	if( 0 == sccb_write_byte ( 0x12, 0x80 ) ) /*��λsensor */
	{
		//DEBUG("sccb write data error");
		return ERROR ;
	}

	if( 0 == sccb_read_byte( &Sensor_IDCode, 1, 0x0b ) )	 /* ��ȡsensor ID��*/
	{
		//DEBUG("read id faild");
		return ERROR;
	}
	//DEBUG("Sensor ID is 0x%x", Sensor_IDCode);

	if(Sensor_IDCode == OV7725_ID)
	{
		for( i = 0 ; i < OV7725_REG_NUM ; i++ )
		{
			if( 0 == sccb_write_byte(Sensor_Config[i].Address, Sensor_Config[i].Value) )
			{
				//DEBUG("write reg faild", Sensor_Config[i].Address);
				return ERROR;
			}
		}

	}
	else
	{
		return ERROR;
	}
	//DEBUG("ov7725 Register Config Success");

	return SUCCESS;
}


/******************************* END OF FILE *********************************/
