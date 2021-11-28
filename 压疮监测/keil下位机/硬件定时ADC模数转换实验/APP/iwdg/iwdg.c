#include "iwdg.h"

IWDG_HandleTypeDef IWDG_Handler; //独立看门狗句柄

//初始化独立看门狗
//pre:分频数:IWDG_PRESCALER_4~IWDG_PRESCALER_256
//rlr:自动重装载值,0~0XFFF.
//时间计算(大概):Tout=((4*2^prer)*rlr)/32 (ms).
void IWDG_Init(u8 pre,u16 rlr)
{
	IWDG_Handler.Instance=IWDG;
    IWDG_Handler.Init.Prescaler=pre;	//设置IWDG分频系数
    IWDG_Handler.Init.Reload=rlr;		//重装载值
    HAL_IWDG_Init(&IWDG_Handler);		//初始化IWDG  
	
    HAL_IWDG_Start(&IWDG_Handler);		//开启独立看门狗
	
}

/*******************************************************************************
* 函 数 名         : IWDG_FeedDog
* 函数功能		   : 喂狗
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void IWDG_FeedDog(void)  //喂狗
{
	HAL_IWDG_Refresh(&IWDG_Handler); 	//喂狗
}

