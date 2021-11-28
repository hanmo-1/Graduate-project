#include "adc.h"
#include "SysTick.h"


ADC_HandleTypeDef ADC1_Handler;//ADC句柄

//初始化ADC
void ADCx_Init(void)
{ 
    ADC1_Handler.Instance=ADC1;
    ADC1_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   //4分频，ADCCLK=PCLK2/4=90/4=22.5MHZ
    ADC1_Handler.Init.Resolution=ADC_RESOLUTION_12B;             //12位模式
    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //右对齐
    ADC1_Handler.Init.ScanConvMode=DISABLE;                      //非扫描模式
    ADC1_Handler.Init.EOCSelection=DISABLE;                      //关闭EOC中断
    ADC1_Handler.Init.ContinuousConvMode=DISABLE;                //关闭连续转换
    ADC1_Handler.Init.NbrOfConversion=1;                         //1个转换在规则序列中 也就是只转换规则序列1 
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;             //禁止不连续采样模式
    ADC1_Handler.Init.NbrOfDiscConversion=0;                     //不连续采样通道数为0
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //软件触发
    ADC1_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//使用软件触发
    ADC1_Handler.Init.DMAContinuousRequests=DISABLE;             //关闭DMA请求
    HAL_ADC_Init(&ADC1_Handler);                                 //初始化 
}

//ADC底层驱动，引脚配置，时钟使能
//此函数会被HAL_ADC_Init()调用
//hadc:ADC句柄
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_ADC1_CLK_ENABLE();            //使能ADC1时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();			//开启GPIOC时钟	
	
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;            //PA5
    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
    GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
	 GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1;            //PB0
	 GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
   GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
	 HAL_GPIO_Init(GPIOB,&GPIO_Initure);     //配置PB0
	
	 GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;            //PC0
	 GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
   GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
	 HAL_GPIO_Init(GPIOC,&GPIO_Initure);     //配置PC0
}

//获得ADC值
//ch: 通道值 0~16，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
//返回值:转换结果
u16 Get_Adc(u32 ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
   switch(ch)
   {
	   case ADC_CHANNEL_5:
		ADC1_ChanConf.Channel=ch;                                   //通道5
		ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
		ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //采样时间
		ADC1_ChanConf.Offset=0;                 
		HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);	   //通道配置
	   ;break;
//	   case ADC_CHANNEL_0:
//	    
//		ADC1_ChanConf.Channel=ch;                                   //通道0
//		ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
//	    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //采样时间
//		ADC1_ChanConf.Offset=0;                 
//	    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //通道配置
//	    ;break;
//		 
//	   case ADC_CHANNEL_1:
//		ADC1_ChanConf.Channel=ch;                                   //通道5
//		ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
//		ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //采样时间
//		ADC1_ChanConf.Offset=0;                 
//		HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);	   //通道配置
//	   ;break;
//		 
//	   case ADC_CHANNEL_2:
//		ADC1_ChanConf.Channel=ch;                                   //通道5
//		ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
//		ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //采样时间
//		ADC1_ChanConf.Offset=0;                 
//		HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);	   //通道配置
//	   ;break;
//		 
//	   case ADC_CHANNEL_3:
//		ADC1_ChanConf.Channel=ch;                                   //通道5
//		ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
//		ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //采样时间
//		ADC1_ChanConf.Offset=0;                 
//		HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);	   //通道配置
//	   ;break;
//		 
//	   case ADC_CHANNEL_4:
//		ADC1_ChanConf.Channel=ch;                                   //通道5
//		ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
//		ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //采样时间
//		ADC1_ChanConf.Offset=0;                 
//		HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);	   //通道配置
//	   ;break;
//		 
//	   case ADC_CHANNEL_6:
//		ADC1_ChanConf.Channel=ch;                                   //通道5
//		ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
//		ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //采样时间
//		ADC1_ChanConf.Offset=0;                 
//		HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);	   //通道配置
//	   ;break;
//		 
//	   case ADC_CHANNEL_7:
//		ADC1_ChanConf.Channel=ch;                                   //通道5
//		ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
//		ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //采样时间
//		ADC1_ChanConf.Offset=0;                 
//		HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);	   //通道配置
//	   ;break;
//		 
//	   	case ADC_CHANNEL_8:
//		ADC1_ChanConf.Channel=ch;                                   //通道5
//		ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
//		ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //采样时间
//		ADC1_ChanConf.Offset=0;                 
//		HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);	   //通道配置
//	   ;break;
//			
//		case ADC_CHANNEL_9:
//		ADC1_ChanConf.Channel=ch;                                   //通道5
//		ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
//		ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //采样时间
//		ADC1_ChanConf.Offset=0;                 
//		HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);	   //通道配置
//	   ;break;
//		
//		case ADC_CHANNEL_10:
//		ADC1_ChanConf.Channel=ch;                                   //通道5
//		ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
//		ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //采样时间
//		ADC1_ChanConf.Offset=0;                 
//		HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);	   //通道配置
//	   ;break;
//		
//		case ADC_CHANNEL_11:
//		ADC1_ChanConf.Channel=ch;                                   //通道5
//		ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
//		ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //采样时间
//		ADC1_ChanConf.Offset=0;                 
//		HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);	   //通道配置
//	   ;break;
//		
//		case ADC_CHANNEL_12:
//		ADC1_ChanConf.Channel=ch;                                   //通道5
//		ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
//		ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //采样时间
//		ADC1_ChanConf.Offset=0;                 
//		HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);	   //通道配置
//	   ;break;
//		
//		case ADC_CHANNEL_14:
//		ADC1_ChanConf.Channel=ch;                                   //通道5
//		ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
//		ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //采样时间
//		ADC1_ChanConf.Offset=0;                 
//		HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);	   //通道配置
//	   ;break;
//		
//		case ADC_CHANNEL_15:
//		ADC1_ChanConf.Channel=ch;                                   //通道5
//		ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
//		ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //采样时间
//		ADC1_ChanConf.Offset=0;                 
//		HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);	   //通道配置
//	   ;break;
		
	   default:  break;
   }
    HAL_ADC_Start(&ADC1_Handler);                               //开启ADC
	
    HAL_ADC_PollForConversion(&ADC1_Handler,10);                //轮询转换
 
	return (u16)HAL_ADC_GetValue(&ADC1_Handler);	        //返回最近一次ADC1规则组的转换结果
}
//获取指定通道的转换值，取times次,然后平均 
//times:获取次数
//返回值:通道ch的times次转换结果平均值
u16 Get_ADC_Value(u32 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 
//u16 Get_ADC_Value_time(u32 ch)
//{
//	return Get_Adc(ch)
//}
