
#include "W25Q64/qspi_w25q64.h"
#include "SDRAM/sdram.h"

void BSP_Init(void)
{
	QSPI_W25Qxx_Init();
	MX_FMC_Init();
}
