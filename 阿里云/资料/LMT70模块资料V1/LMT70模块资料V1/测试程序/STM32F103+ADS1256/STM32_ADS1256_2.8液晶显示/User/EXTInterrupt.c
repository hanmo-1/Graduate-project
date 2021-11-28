//-----------------------------------------------------------------
// 程序描述:
//		- 外中断驱动程序
// 作    者: 凌智电子
// 开始日期: 2014-02-02
// 完成日期: 2014-02-02
// 修改日期: 2014-02-20
// 版    本: V1.1
//   - V1.0: 使用外中断线实现中断
//   - V1.1: 修改格式和函数名
// 调试工具: 凌智STM32+FPGA电子系统设计开发板、LZE_ST LINK2
// 说    明:
//    - 调试使用的系统时钟频率Fsysclk=72MHz；
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include <stm32f10x.h>
#include "EXTInterrupt.h"

//-----------------------------------------------------------------
// void EXTInterrupt_Mode_Configuration(void)
//-----------------------------------------------------------------
//
// 函数功能: 外中断的配置
// 入口参数: 无
// 返 回 值: 无
// 注意事项:
//-----------------------------------------------------------------
void EXTInterrupt_Mode_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	// PB.6与中断线6相映射
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource15);          
	// PB.6对应中断线6//外中断//下降沿触发中断
	EXTI_InitStructure.EXTI_Line=EXTI_Line15;                            
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;									
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;							
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);	 	
	
}

//-----------------------------------------------------------------
// void NVIC_Configuration(void)
//-----------------------------------------------------------------
//
// 函数功能: 配置外中断嵌套中断向量
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 
//-----------------------------------------------------------------
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  // 设置NVIC中断分组2,其中2位抢占优先级，2位响应优先级
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);												
  // 选择外部中断通道//抢占优先级0//子优先级0
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;										
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;							
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;										
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

//-----------------------------------------------------------------
// void Exti_Init(void)
//-----------------------------------------------------------------
//
// 函数功能: 外中断初始化
// 入口参数: 无
// 返 回 值: 无
// 注意事项:
//-----------------------------------------------------------------
void EXTInterrupt_Init(void)
{
	NVIC_Configuration();
	EXTInterrupt_Mode_Configuration();
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
