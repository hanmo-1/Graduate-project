//-----------------------------------------------------------------
// 程序描述:
// 　　LMT70测温程序
// 作　　者: 凌智电子
// 开始日期: 2020-09-05
// 完成日期: 2020-09-09
// 修改日期: 2020-09-09
// 版　　本: V1.0
// 　- V1.0:  
// 调试工具: 凌智STM32核心开发板、2.8寸液晶屏、LZE_ST_LINK2、ADS1256模块
// 说　　明: 
//       (1) ADS1256模块和STM32核心板的连接
//							ADS1256模块    	STM32核心板   
//     					 GND   -------  GND       
//     					 DRDY  ------>  PC9       
//     					 CS    <------  PC11    
//     					 DIN   <------  PC10     
//     					 DOUT  ------>  PC12      
//    					 SCLK  <------  PC8     
//     					 GND   -------  GND           
//     					 RST   <------  PC13
//							 VCC	 -------  5V
//			 (1) ADS1256模块和LMT70模块的连接
//					 		ADS1256模块		LMT70模块
//							IN0						TAO
//							VCC						VCC
//							GND						GND

//-----------------------------------------------------------------

#include "stm32f10x.h"
#include "Delay.h"
#include "PeripheralInit.h"
#include "ads1256.h"
#include "lze_lcd.h"
#include "stdio.h"
#include "USART.h"
//-----------------------------------------------------------------
// 全局变量
//-----------------------------------------------------------------
// 接收成功标志
extern u8 Resive_Flag;
// 接收缓冲
// extern uint8_t USB_Rx_Buffer[VIRTUAL_COM_PORT_DATA_SIZE];

u8 IO_TAG=1;

//-----------------------------------------------------------------
// void USB_Send_string(u8* data_buffer)
//-----------------------------------------------------------------
//
// 函数功能:	USB发送字符串
// 入口参数:  发送字符串首地址
// 返 回 值:  无
// 注意事项:
//-----------------------------------------------------------------

// void USB_Send_string(u8* data_buffer)
// {  
//   // 拷贝用户数据到端点1的发送缓冲区
//   UserToPMABufferCopy(data_buffer, ENDP1_TXADDR, 39); 
// 	// 设置端点1发送数据长度
//   SetEPTxCount(ENDP1, 39); 
// 	// 设置端点1的发送状态为Valid
//   SetEPTxValid(ENDP1);		
// }

int main(void)
{
	u8 dis_buf[40];									// 显示缓冲区
	int32_t adc[8];									// 采样结果
	int32_t adc_1[8];								//
	float volt[8];									// 实际电压值
	float T;
//	uint8_t i;
// uint8_t ch_num=8;									//通道数
	
	PeripheralInit();								// 各种初始化
	LCD_Clear(LCD_COLOR_BLACK); 		// 设置背景色
	
	ADS1256_CfgADC(PGA_1, DATARATE_100);	//配置ADC参数： 增益1:1, 数据输出速率 15Hz 
	
	ADS1256_StartScan(0);	//启动中断扫描模式. 0表示单端8路，1表示差分4路

	LCD_WriteString(10,10,LCD_COLOR_YELLOW,LCD_COLOR_BLACK,(uint8_t *)"STM32 LMT70 Temperature Test:");
	LCD_WriteString(10,50,LCD_COLOR_BLUE,LCD_COLOR_BLACK,(uint8_t *)"ADC Ch0 Voltage:          mV");
	LCD_WriteString(10,90,LCD_COLOR_BLUE,LCD_COLOR_BLACK,(uint8_t *)"    Temperature:          C");

		
	while(1)
	{
//		for (i = 0; i < ch_num; i++)
//		{
//			adc[i] = (int32_t)ADS1256_GetAdc(i);		                 //从全局缓冲区读取采样结果。 采样结果是在中断服务程序中读取的
//			adc_1[i] = (adc[i] & 0x7FFFFFFF);				                 //4194303 = 2.5V , 这是理论值，实际可以根据2.5V基准的实际值进行公式矫正 

//			if(adc[i]&0x80000000)
//				volt[i] =((((float)adc_1[i] * 5000) / 8388608))-5000;	//计算实际电压值（近似估算的），如需准确，请进行校准 
//			else
//				volt[i] =((((float)adc_1[i] * 5000) / 8388608)-0.15);			//计算实际电压值（近似估算的），如需准确，请进行校准 
//		}
	

			adc[0] = (int32_t)ADS1256_GetAdc(0);		                 //从全局缓冲区读取采样结果。 采样结果是在中断服务程序中读取的
			adc_1[0] = (adc[0] & 0x7FFFFFFF);				                 //4194303 = 2.5V , 这是理论值，实际可以根据2.5V基准的实际值进行公式矫正 

			if(adc[0]&0x80000000)
				volt[0] =((((float)adc_1[0] * 5000) / 8388608))-5000;	//计算实际电压值（近似估算的），如需准确，请进行校准 
			else
				volt[0] =((((float)adc_1[0] * 5000) / 8388608)-0.15);			//计算实际电压值（近似估算的），如需准确，请进行校准 

		
		LCD_WriteString(190,140,LCD_COLOR_BLUE,LCD_COLOR_BLACK,(u8 *)"         ");
		LCD_WriteString(170,140,LCD_COLOR_BLUE,LCD_COLOR_BLACK,(u8 *)"         ");

		sprintf((char*)dis_buf,"%.3f", (float)volt[0]);		//液晶显示
		LCD_WriteString(155,50,LCD_COLOR_RED,LCD_COLOR_BLACK,dis_buf );   
		T=(-0.193)*volt[0]+212.009;
		sprintf((char*)dis_buf,"%.2f", (float)T);
		LCD_WriteString(155,90,LCD_COLOR_RED,LCD_COLOR_BLACK,dis_buf );   
		Delay_5ms(60);
	}
}


//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
