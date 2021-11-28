//-----------------------------------------------------------------
// 程序描述:
// 　　ADC驱动程序
// 作　　者: 凌智电子
// 开始日期: 2014-02-18
// 完成日期: 2014-02-18
// 修改日期: 2014-02-18
// 版　　本: V1.0
// 　- V1.0:
// 调试工具: 凌智STM32核心开发板、2.8寸液晶屏、LZE_ST_LINK2
// 说　　明:
//
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include <stm32f10x.h>
#include "ADCx_RegularChannel.h"

//-----------------------------------------------------------------
// 全局变量定义
//-----------------------------------------------------------------
__IO uint16_t ADC_RegularConvertedValueTab[Channel];

//-----------------------------------------------------------------
// 初始化程序区
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// void GPIO_ADC_Configuration(void)
//-----------------------------------------------------------------
//
// 函数功能: ADC通道GPIO配置
// 输入参数: 无
// 输出参数: 无
// 返 回 值: 无
// 全局变量: 无
// 调用函数: 无
// 注意事项: 规则组通道最多16个，注入组通道最多4个
//          PC0-->ADC通道10,模拟输入
//           PC1-->ADC通道11,模拟输入
//          PC2-->ADC通道12,模拟输入
//          PC3-->ADC通道13,模拟输入
//          PC4-->ADC通道14,模拟输入
//          PC5-->ADC通道15,模拟输入
//          PB0-->ADC通道8 ,模拟输入
//          PB1-->ADC通道9 ,模拟输入
//
//-----------------------------------------------------------------
void GPIO_ADC_Configuration(ADC_TypeDef *ADCx)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

//-----------------------------------------------------------------
// void ADC_Initialization(ADC_TypeDef* ADCx)
//-----------------------------------------------------------------
//
// 函数功能: ADC初始化
// 入口参数: 无
// 返回参数: 无
// 全局变量: 无
// 调用模块: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void ADC_Initialization(ADC_TypeDef *ADCx)
{
  ADC_InitTypeDef ADC_InitStructure;
  // 独立 ADC 模式
  ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;
  // 单通道模式
  ADC_InitStructure.ADC_ScanConvMode       = DISABLE;
  // 连续转换模式
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  // 使用外部触发来启动规则通道的模数转换
  ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
  // 采集数据右对齐
  ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
  // 规定了顺序进行规则转换的ADC通道的数目:1
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
// 全局变量: 无
// 调用模块: ADC_ResetCalibration(); DC_GetResetCalibrationStatus();
//          ADC_StartCalibration();ADC_GetCalibrationStatus();
// 注意事项:
//
//-----------------------------------------------------------------
void ADC_Calibration(ADC_TypeDef *ADCx)
{
  // 复位ADC自校准
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
// 全局变量: 无
// 调用模块: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void DMA1_ADC_Configuration(void)
{
  DMA_InitTypeDef DMA_InitStructure;
  // 使能外设DMA1时钟
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  // 复位DMA
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
  // 设置了DMA的工作模式: 工作在循环缓存模式
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
  // 设定 DMA通道 x 的软件优先级:高优先级
  DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
  // 失能DMA通道的内存到内存传输
  DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
  // 初始化DMA寄存器
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  // 使能DMA通道1
  DMA_Cmd(DMA1_Channel1, ENABLE);
}

//-----------------------------------------------------------------
// void ADC_RegularChannel_Configuration(ADC_TypeDef* ADCx)
//-----------------------------------------------------------------
//
// 函数功能: ADC规则组配置函数
// 入口参数: 无
// 返回参数: 无
// 全局变量: 无
// 调用模块:
// 注意事项: ADC最大时钟不超过14MHz,这里设置为系统时钟的6分频，即72M/6=12MHz
//
//-----------------------------------------------------------------
void ADC_RegularChannel_Configuration(ADC_TypeDef *ADCx)
{
  // 打开ADC1时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 |RCC_APB2Periph_ADC2, ENABLE);
  RCC_ADCCLKConfig(RCC_PCLK2_Div8); // 设置 ADC时钟，为PCLK2的8分频,
  GPIO_ADC_Configuration(ADCx);     // ADCx IO配置
  DMA1_ADC_Configuration();         // 配置ADC在DMA1中的通道
  ADC_Initialization(ADCx);         // ADCx初始化
  // 选择ADC1通道14，排在第一位采集，采样时间为239.5周期:测量LMT70输出电压
  ADC_RegularChannelConfig(ADCx, ADC_Channel_14, 1, ADC_SampleTime_239Cycles5);
  ADC_DMACmd(ADCx, ENABLE);               // 使能ADCx的DMA通道
  ADC_TempSensorVrefintCmd(ENABLE);       // 使能基准源通道17
  ADC_Cmd(ADCx, ENABLE);                  // 使能ADCx
  ADC_Calibration(ADCx);                  // ADCx自动校准
  ADC_SoftwareStartConvCmd(ADCx, ENABLE); // 启动ADCx软件转换
}


//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
