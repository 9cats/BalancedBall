#include "main.h"

static uint16_t fac_us = 0;

/**
 * @brief	��ʼ���ӳٺ���,SYSTICK��ʱ�ӹ̶�ΪAHBʱ��
 * @param   SYSCLK	ϵͳʱ��Ƶ��
 * @return  void
 */
void delay_init(uint16_t SYSCLK)
{
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);//SysTickƵ��ΪHCLK
    fac_us = SYSCLK;						//�����Ƿ�ʹ��OS,fac_us����Ҫʹ��
}

/**
 *  @brief  �ӳ�us
 *  @param  t us��
 *  @note   delay_init��ʹ��
 */
void delay_us(uint32_t nus) {
	uint32_t ticks;
	uint32_t told, tnow, tcnt = 0;
	uint32_t reload = SysTick->LOAD; //LOAD��ֵ
	ticks = nus * fac_us;       //��Ҫ�Ľ�����
	told = SysTick->VAL;        //�ս���ʱ�ļ�����ֵ
	while (1) {
		tnow = SysTick->VAL;
		if (tnow != told) {
			if (tnow < told)
				tcnt += told - tnow; //����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else
				tcnt += reload - tnow + told;
			told = tnow;
			if (tcnt >= ticks)
				break; //ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}
	};
}

/**
 *  @brief  �ӳ�ms
 *  @param  t ms��
 *  @note   delay_init��ʹ��
 */
void delay_ms(uint16_t nms) {
	delay_us(nms*1000);
}
