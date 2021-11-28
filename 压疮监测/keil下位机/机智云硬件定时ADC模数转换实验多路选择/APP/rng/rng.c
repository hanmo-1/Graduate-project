#include "rng.h"
#include "SysTick.h"


RNG_HandleTypeDef RNG_Handler;  //RNG句柄

/*******************************************************************************
* 函 数 名         : RNG_Init
* 函数功能		   : RNG初始化函数
* 输    入         : 无
* 输    出         : 1：错误，0：成功
*******************************************************************************/
u8 RNG_Init(void)
{
	u16 i=0;
    
    RNG_Handler.Instance=RNG;
    HAL_RNG_Init(&RNG_Handler);//初始化RNG
    while(__HAL_RNG_GET_FLAG(&RNG_Handler,RNG_FLAG_DRDY)==RESET&&i<10000)//等待RNG准备就绪
    {
        i++;
        delay_us(10);
    }
    if(i>=10000) 
		return 1;//随机数产生器工作不正常
    return 0;
}

void HAL_RNG_MspInit(RNG_HandleTypeDef *hrng)
{
     __HAL_RCC_RNG_CLK_ENABLE();//使能RNG时钟
}
//得到随机数
//返回值:获取到的随机数
u32 RNG_Get_RandomNum(void)
{
    return HAL_RNG_GetRandomNumber(&RNG_Handler);
}

//生成[min,max]范围的随机数
int RNG_Get_RandomRange(int min,int max)
{ 
   return HAL_RNG_GetRandomNumber(&RNG_Handler)%(max-min+1) +min;
}
