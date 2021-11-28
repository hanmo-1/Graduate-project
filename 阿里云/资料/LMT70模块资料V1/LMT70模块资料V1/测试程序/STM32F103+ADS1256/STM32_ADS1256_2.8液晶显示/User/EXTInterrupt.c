//-----------------------------------------------------------------
// ��������:
//		- ���ж���������
// ��    ��: ���ǵ���
// ��ʼ����: 2014-02-02
// �������: 2014-02-02
// �޸�����: 2014-02-20
// ��    ��: V1.1
//   - V1.0: ʹ�����ж���ʵ���ж�
//   - V1.1: �޸ĸ�ʽ�ͺ�����
// ���Թ���: ����STM32+FPGA����ϵͳ��ƿ����塢LZE_ST LINK2
// ˵    ��:
//    - ����ʹ�õ�ϵͳʱ��Ƶ��Fsysclk=72MHz��
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include <stm32f10x.h>
#include "EXTInterrupt.h"

//-----------------------------------------------------------------
// void EXTInterrupt_Mode_Configuration(void)
//-----------------------------------------------------------------
//
// ��������: ���жϵ�����
// ��ڲ���: ��
// �� �� ֵ: ��
// ע������:
//-----------------------------------------------------------------
void EXTInterrupt_Mode_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	// PB.6���ж���6��ӳ��
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource15);          
	// PB.6��Ӧ�ж���6//���ж�//�½��ش����ж�
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
// ��������: �������ж�Ƕ���ж�����
// ��ڲ���: ��
// �� �� ֵ: ��
// ע������: 
//-----------------------------------------------------------------
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  // ����NVIC�жϷ���2,����2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);												
  // ѡ���ⲿ�ж�ͨ��//��ռ���ȼ�0//�����ȼ�0
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
// ��������: ���жϳ�ʼ��
// ��ڲ���: ��
// �� �� ֵ: ��
// ע������:
//-----------------------------------------------------------------
void EXTInterrupt_Init(void)
{
	NVIC_Configuration();
	EXTInterrupt_Mode_Configuration();
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
