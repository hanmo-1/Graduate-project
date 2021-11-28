#include "iwdg.h"

IWDG_HandleTypeDef IWDG_Handler; //�������Ź����

//��ʼ���������Ź�
//pre:��Ƶ��:IWDG_PRESCALER_4~IWDG_PRESCALER_256
//rlr:�Զ���װ��ֵ,0~0XFFF.
//ʱ�����(���):Tout=((4*2^prer)*rlr)/32 (ms).
void IWDG_Init(u8 pre,u16 rlr)
{
	IWDG_Handler.Instance=IWDG;
    IWDG_Handler.Init.Prescaler=pre;	//����IWDG��Ƶϵ��
    IWDG_Handler.Init.Reload=rlr;		//��װ��ֵ
    HAL_IWDG_Init(&IWDG_Handler);		//��ʼ��IWDG  
	
    HAL_IWDG_Start(&IWDG_Handler);		//�����������Ź�
	
}

/*******************************************************************************
* �� �� ��         : IWDG_FeedDog
* ��������		   : ι��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void IWDG_FeedDog(void)  //ι��
{
	HAL_IWDG_Refresh(&IWDG_Handler); 	//ι��
}

