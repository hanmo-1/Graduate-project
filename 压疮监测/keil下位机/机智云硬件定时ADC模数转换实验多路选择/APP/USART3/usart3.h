#ifndef __USART3_H
#define __USART3_H	 
#include "system.h" 

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F429开发板
//串口3初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.csom
//修改日期:2016/6/17
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************
//V1.0修改说明 
////////////////////////////////////////////////////////////////////////////////// 	

//UART句柄
extern UART_HandleTypeDef UART3_Handler; 

void uart3_init(u32 bound);				//串口3初始化 


#endif



