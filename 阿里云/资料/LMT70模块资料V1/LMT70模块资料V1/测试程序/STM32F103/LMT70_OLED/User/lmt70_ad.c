//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "stm32f10x.h"
#include "lmt70_ad.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"
#include "delay.h"
//-----------------------------------------------------------------
// 全局变量定义
//-----------------------------------------------------------------
double temp,receive;          //temp计算温度，receive接收电压

__IO uint16_t ADC_RegularConvertedValueTab[Channel];

//-----------------------------------------------------------------
// void GPIO_ADC_Configuration(void)
//-----------------------------------------------------------------
//
// 函数功能: ADC通道GPIO配置
// 输入参数: 无
// 输出参数: 无
// 返 回 值: 无
// 注意事项: PA1-->ADC通道1,模拟输入                 
//-----------------------------------------------------------------
void GPIO_ADC_Configuration()
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}
//-----------------------------------------------------------------
// void ADC_Initialization(ADC_TypeDef* ADCx)
//-----------------------------------------------------------------
//
// 函数功能: ADC初始化
// 入口参数: 无
// 返回参数: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void ADC_Initialization(ADC_TypeDef *ADCx)
{
  ADC_InitTypeDef ADC_InitStructure;
  // 设置每个ADC工作在独立模式
  ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;
  // 规定了模数转换工作单通道模式
  ADC_InitStructure.ADC_ScanConvMode       = DISABLE;
  // 规定了模数转换工作在连续转换模式
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  // 规则通道
  ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
  // 右对齐
  ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
  //数目：1
  ADC_InitStructure.ADC_NbrOfChannel       = Channel;
  // 设置ADC1的寄存器
  ADC_Init(ADCx, &ADC_InitStructure);
}
//-----------------------------------------------------------------
// void ADC_Calibration(ADC_TypeDef * ADCx)
//-----------------------------------------------------------------
//
// 函数功能: ADC自动校准函数
// 入口参数: 无
// 返回参数: 无
// 注意事项:
//
//-----------------------------------------------------------------
void ADC_Calibration(ADC_TypeDef *ADCx)
{
  ADC_ResetCalibration(ADCx);
	// 检查ADC复位校准结束否
  while (ADC_GetResetCalibrationStatus(ADCx))
    ;
	  // 启动ADC自校准程序
  ADC_StartCalibration(ADCx);
   // 检查ADC自校准结束否
  while (ADC_GetCalibrationStatus(ADCx))
    ;
}
//-----------------------------------------------------------------
// void DMA1_ADC_Configuration(void)
//-----------------------------------------------------------------
//
// 函数功能: ADC的DMA通道配置
// 入口参数: 无
// 返回参数: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void DMA1_ADC_Configuration(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  DMA_DeInit(DMA1_Channel1);
  // 定义DMA外设ADC基地址
   DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  // 定义DMA内存基地址
  DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)ADC_RegularConvertedValueTab;
  // 规定了外设是作为数据传输的目的地还是来源，数据从外设输出
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
  // 定义指定DMA通道的DMA缓存的大小,单位为数据单位
  DMA_InitStructure.DMA_BufferSize         = Channel;
  // 设定外设地址寄存器递增与否
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  // 设定内存地址寄存器递增与否
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  // 设定了外设数据宽度 ，数据宽度为16位,ADC为12位分辨率
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  // 设定了内存数据宽度  16
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
  //循环缓存
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
  // 设定 DMA通道 x 的软件优先级:高优先级
  DMA_InitStructure.DMA_Priority           = DMA_Priority_High;

  DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
  // 初始化DMA寄存器
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  DMA_Cmd(DMA1_Channel1, ENABLE);
}

//-----------------------------------------------------------------
// void ADC_RegularChannel_Configuration(ADC_TypeDef* ADCx)
//-----------------------------------------------------------------
//
// 函数功能: ADC规则组配置函数
// 入口参数: 无
// 返回参数: 无
// 注意事项: ADC最大时钟不超过14MHz,这里设置为系统时钟的6分频，即72M/6=12MHz
//
//-----------------------------------------------------------------
void ADC_RegularChannel_Configuration(ADC_TypeDef *ADCx)
{
  // 打开ADC1时钟
	GPIO_ADC_Configuration();          // ADCx IO配置
	DMA1_ADC_Configuration();          // 配置ADC在DMA1中的通道
  ADC_Calibration(ADCx);             // ADCx初始化
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
  RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 设置 ADC时钟
  ADC_Initialization(ADCx);         
  // 选择ADC1通道1，排在第一位采集，采样时间为41.5周期:测量DA通道1输出电压
  ADC_RegularChannelConfig(ADCx, ADC_Channel_1, 1, ADC_SampleTime_41Cycles5);
  ADC_DMACmd(ADCx, ENABLE);               
  ADC_TempSensorVrefintCmd(ENABLE);       
  ADC_Cmd(ADCx, ENABLE);                  // 使能ADCx
  ADC_Calibration(ADCx);                  // ADCx自动校准
  ADC_SoftwareStartConvCmd(ADCx, ENABLE); // 启动ADCx软件转换
}
//-----------------------------------------------------------------
// void average(void)
//-----------------------------------------------------------------
//
// 函数功能: 取100个接受温度取平均函数
// 入口参数: 无
// 返回参数: 无
// 注意事项: 
//
//-----------------------------------------------------------------
void average(void)
{
 u8 i;
 u32 tem[200];
	Delay_5ms(5);
	for(i=0;i<200;i++)
	{
     tem[i]=AnalogVoltage(0);		 
		 receive+=tem[i];
	
  }
		 receive/=200000;
   
}


//-----------------------------------------------------------------
// void caculation(void)
//-----------------------------------------------------------------
//
// 函数功能: 温度计算函数
// 入口参数: 无
// 返回参数: 无
// 注意事项: 
//
//-----------------------------------------------------------------
void caculation(void)
{
	   average();
   //计算温度
    temp=204.6398-receive*receive*7.857923/1000000-receive*0.1777501;
	
	       
}


//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------



