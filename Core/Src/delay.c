#include "main.h"

static uint16_t fac_us = 0;

/**
 * @brief	初始化延迟函数,SYSTICK的时钟固定为AHB时钟
 * @param   SYSCLK	系统时钟频率
 * @return  void
 */
void delay_init(uint16_t SYSCLK)
{
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);//SysTick频率为HCLK
    fac_us = SYSCLK;						//不论是否使用OS,fac_us都需要使用
}

/**
 *  @brief  延迟us
 *  @param  t us数
 *  @note   delay_init后使用
 */
void delay_us(uint32_t nus) {
	uint32_t ticks;
	uint32_t told, tnow, tcnt = 0;
	uint32_t reload = SysTick->LOAD; //LOAD的值
	ticks = nus * fac_us;       //需要的节拍数
	told = SysTick->VAL;        //刚进入时的计数器值
	while (1) {
		tnow = SysTick->VAL;
		if (tnow != told) {
			if (tnow < told)
				tcnt += told - tnow; //这里注意一下SYSTICK是一个递减的计数器就可以了.
			else
				tcnt += reload - tnow + told;
			told = tnow;
			if (tcnt >= ticks)
				break; //时间超过/等于要延迟的时间,则退出.
		}
	};
}

/**
 *  @brief  延迟ms
 *  @param  t ms数
 *  @note   delay_init后使用
 */
void delay_ms(uint16_t nms) {
	delay_us(nms*1000);
}
