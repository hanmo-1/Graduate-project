#include "rng.h"
#include "SysTick.h"


RNG_HandleTypeDef RNG_Handler;  //RNG���

/*******************************************************************************
* �� �� ��         : RNG_Init
* ��������		   : RNG��ʼ������
* ��    ��         : ��
* ��    ��         : 1������0���ɹ�
*******************************************************************************/
u8 RNG_Init(void)
{
	u16 i=0;
    
    RNG_Handler.Instance=RNG;
    HAL_RNG_Init(&RNG_Handler);//��ʼ��RNG
    while(__HAL_RNG_GET_FLAG(&RNG_Handler,RNG_FLAG_DRDY)==RESET&&i<10000)//�ȴ�RNG׼������
    {
        i++;
        delay_us(10);
    }
    if(i>=10000) 
		return 1;//���������������������
    return 0;
}

void HAL_RNG_MspInit(RNG_HandleTypeDef *hrng)
{
     __HAL_RCC_RNG_CLK_ENABLE();//ʹ��RNGʱ��
}
//�õ������
//����ֵ:��ȡ���������
u32 RNG_Get_RandomNum(void)
{
    return HAL_RNG_GetRandomNumber(&RNG_Handler);
}

//����[min,max]��Χ�������
int RNG_Get_RandomRange(int min,int max)
{ 
   return HAL_RNG_GetRandomNumber(&RNG_Handler)%(max-min+1) +min;
}
