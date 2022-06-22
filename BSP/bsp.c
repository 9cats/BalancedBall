
#include "W25Q64/qspi_w25q64.h"
#include "SDRAM/sdram.h"
#include "LCD/lcd_rgb.h"
#include "LCD/touch.h"

void BSP_Init(void)
{
	QSPI_W25Qxx_Init();
	MX_FMC_Init();
	MX_LTDC_Init();
	Touch_Init();
}
