/*********************** HNIT 3103 Application Team **************************
 * 文 件 名 ：hnit_ov7725.c
 * 描    述 ：ov7725摄像头驱动
 * 实验平台 ：STM32F407开发板
 * 库 版 本 ：ST1.4.0
 * 时    间 ：2016.04.08
 * 作    者 ：3103创新团队 王昱霏
 * 修改记录 ：无
******************************************************************************/
#include "ov7725_sccb.h"
#include "ov7725.h"

/**
  * @brief  OV7725摄像头初始化
  * @param  无
  * @retval SUCCESS: 成功
  *         ERROR  : 失败
  */
ErrorStatus ov7725_init(void)
{
	if(SCCB_WR_Reg(0x12,0x80))
	{
		return ERROR;
	}
	return SUCCESS;	//软复位OV7725
}



/******************************* END OF FILE *********************************/
