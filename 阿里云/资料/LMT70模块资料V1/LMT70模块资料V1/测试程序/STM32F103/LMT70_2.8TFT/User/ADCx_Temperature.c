//-----------------------------------------------------------------
// 程序描述:
// 　　ADC驱动程序
// 作　　者: 凌智电子
// 开始日期: 2014-02-17
// 完成日期: 2014-02-17
// 修改日期: 2014-02-17
// 版　　本: V1.0
// 　- V1.0:
// 调试工具: 凌智STM32+FPGA电子系统设计开发板、2.8寸液晶屏、
//            LZE_ST_LINK2
// 说　　明:
//
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include <stm32f10x.h>
#include "ADCx_Temperature.h"

//-----------------------------------------------------------------
// 全局变量定义
//-----------------------------------------------------------------
__IO uint16_t ADCConvertedValue;

//-----------------------------------------------------------------
// 初始化程序区
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// void GPIO_ADC_Configuration(ADC_TypeDef* ADCx)
//-----------------------------------------------------------------
//
// 函数功能: ADC引脚配置函数
// 入口参数: 无
// 返回参数: 无
// 全局变量: 无
// 调用模块: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void GPIO_ADC_Configuration(ADC_TypeDef *ADCx)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
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
  // ADC1和ADC2工作在独立模式
  ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;
  // 禁止多通道
  ADC_InitStructure.ADC_ScanConvMode       = DISABLE;
  // 连续转换模式
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  // 由软件触发转换
  ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
  // 数据右对齐
  ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
  // 规定规则组转换的通道数目
  ADC_InitStructure.ADC_NbrOfChannel       = 1;
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
  // 启动ADC1自校准程序
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
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  //DMA通道1配置
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)&ADCConvertedValue;
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize         = 1;
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  // 使能DMA1通道1
  DMA_Cmd(DMA1_Channel1, ENABLE);
}

//-----------------------------------------------------------------
// void ADC_Temperature_Configuration(ADC_TypeDef* ADCx)
//-----------------------------------------------------------------
//
// 函数功能: ADC配置函数
// 入口参数: 无
// 返回参数: 无
// 全局变量: 无
// 调用模块: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void ADC_Temperature_Configuration(ADC_TypeDef *ADCx)
{
  // 打开ADC时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  // ADCx IO口配置
  GPIO_ADC_Configuration(ADCx);
  // 配置ADC在DMA1中的通道
  DMA1_ADC_Configuration();
  // ADCx初始化
  ADC_Initialization(ADCx);
  // 选中ADCx通道16温度传感器模块作为输入
  ADC_RegularChannelConfig(ADCx, ADC_Channel_16, 1, ADC_SampleTime_55Cycles5);
  // 使能ADCx的DMA通道
  ADC_DMACmd(ADCx, ENABLE);
  // 使能温度传感器
  ADC_TempSensorVrefintCmd(ENABLE);
  // 使能ADCx
  ADC_Cmd(ADCx, ENABLE);
  // ADCx自动校准
  ADC_Calibration(ADCx);
  // 启动ADCx软件转换
  ADC_SoftwareStartConvCmd(ADCx, ENABLE);
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
