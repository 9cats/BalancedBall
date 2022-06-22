#ifndef __FONTS_H
#define __FONTS_H


#include <stdint.h>

/*------------------------------------ ASCII���� ---------------------------------------------*/

// ������ؽṹ����
typedef struct _pFont
{
	const uint8_t 		*pTable;  		//	��ģ�����ַ
	uint16_t 			Width; 		 	//	�����ַ�����ģ���
	uint16_t 			Height; 			//	�����ַ�����ģ����
	uint16_t 			Sizes;	 		//	�����ַ�����ģ���ݸ���
	uint16_t				Table_Rows;		// �ò���ֻ�к�����ģ�õ�����ʾ��ά������д�С
} pFONT;

extern pFONT Font32;	// 3216 ����
extern pFONT Font24;	// 2412 ����
extern pFONT Font20; // 2010 ����
extern pFONT Font16; // 1608 ����
extern pFONT Font12; // 1206 ����


/*------------------------------------ �������� ---------------------------------------------*/

typedef struct _pChineseFont
{
	uint16_t Width; 			// ��ģ���
	uint16_t Height;		 	// ��ģ����
	uint16_t Sizes;	 		// ��ģ����
	uint32_t FontAddress;	// �ֿ��ļ�·��
} pChineseFont;

extern pChineseFont CH_Font12 ;		// 1212����
extern pChineseFont CH_Font16 ;		// 1616����
extern pChineseFont CH_Font24 ;		// 2424����
extern pChineseFont CH_Font32 ;		// 3232����

#define  FontAddress_CH_12  0x00000000		// ����1212�ֿ���SPI Flash �е���ʼ��ַ
#define  FontAddress_CH_16  0x00030000		// ����1616�ֿ���SPI Flash �е���ʼ��ַ
#define  FontAddress_CH_24  0x00070000		// ����2424�ֿ���SPI Flash �е���ʼ��ַ
#define  FontAddress_CH_32  0x00100000		// ����3232�ֿ���SPI Flash �е���ʼ��ַ



#endif

