//-----------------------------------------------------------------
// 程序描述:
//    	延时程序头文件
// 作    者: 凌智电子
// 开始日期: 2018-08-04
// 完成日期: 2018-08-04
// 当前版本: V1.0
// 历史版本:
//   - V1.0: 基于STM32的延时：us,ms
// 调试工具: 凌智STM32F429+CycloneIV电子系统设计开发板、LZE_ST_LINK2
// 说    明: 
//
//-----------------------------------------------------------------
#ifndef _DELAY_H
#define _DELAY_H
#include "stm32f429_winner.h"
//-----------------------------------------------------------------
// 定义
//-----------------------------------------------------------------
#define SystemClock				180				// 系统时钟（单位：MHz）
#define FAC_us				SystemClock/4	// 延时因子
//-----------------------------------------------------------------
// 函数声明
//-----------------------------------------------------------------
extern void delay_20ns(u32 nns);
extern void delay_ms(u16 nms);
extern void delay_us(u32 nus);
extern void SysTick_clkconfig(u8 SYSCLK);
extern void SysTick_delay_us(u32 nus);
extern void SysTick_delay_ms(u16 nms);
#endif
//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
