#include "touch_key.h"
#include "SysTick.h"
#include "usart.h"


TIM_HandleTypeDef TIM2_Handler;         //��ʱ��2��� 

#define Touch_ARR_MAX_VAL 0xffffffff  //����ARRֵ(TIM2��32λ��ʱ��)	
u16 touch_default_val=0;  //Ϊ���´�������ʱ��ֵ

/*******************************************************************************
* �� �� ��         : TIM2_CH1_Input_Init
* ��������		   : TIM2_CH1���벶���ʼ������
* ��    ��         : arr���Զ���װ��ֵ
					 psc��Ԥ��Ƶϵ��
* ��    ��         : ��
*******************************************************************************/
void TIM2_CH1_Input_Init(u32 arr,u16 psc)
{
	TIM_IC_InitTypeDef TIM2_CH1Config;  
    
    TIM2_Handler.Instance=TIM2;                          //ͨ�ö�ʱ��3
    TIM2_Handler.Init.Prescaler=psc;                     //��Ƶ
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM2_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_IC_Init(&TIM2_Handler);
    
    TIM2_CH1Config.ICPolarity=TIM_ICPOLARITY_RISING;    //�����ز���
    TIM2_CH1Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//ӳ�䵽TI1��
    TIM2_CH1Config.ICPrescaler=TIM_ICPSC_DIV1;          //���������Ƶ������Ƶ
    TIM2_CH1Config.ICFilter=0;                          //���������˲��������˲�
    HAL_TIM_IC_ConfigChannel(&TIM2_Handler,&TIM2_CH1Config,TIM_CHANNEL_1);//����TIM2ͨ��1
    HAL_TIM_IC_Start(&TIM2_Handler,TIM_CHANNEL_1);      //��ʼ����TIM2��ͨ��1
}

//��ʱ��2�ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻHAL_TIM_IC_Init()����
//htim:��ʱ��2���
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_TIM2_CLK_ENABLE();            //ʹ��TIM2ʱ��
    __HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOAʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_5;            //PA5
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //���츴��
    GPIO_Initure.Pull=GPIO_NOPULL;          //����������
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    GPIO_Initure.Alternate=GPIO_AF1_TIM2;   //PA5����ΪTIM2ͨ��1
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}

/*******************************************************************************
* �� �� ��         : Touch_Reset
* ��������		   : ����������λ �ȷŵ�Ȼ���粢�ͷż�ʱ���ڵ�ֵ
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Touch_Reset(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.Pin=GPIO_PIN_5;            //PA5
    GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_InitStructure.Pull=GPIO_PULLDOWN;        //����
    GPIO_InitStructure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);	//PA5���0���ŵ�

	delay_ms(5);
	__HAL_TIM_CLEAR_FLAG(&TIM2_Handler,TIM_FLAG_CC1|TIM_FLAG_UPDATE);   //�����־λ
    __HAL_TIM_SET_COUNTER(&TIM2_Handler,0); //������ֵ��0
    
    GPIO_InitStructure.Mode=GPIO_MODE_AF_PP;      //���츴��
    GPIO_InitStructure.Pull=GPIO_NOPULL;          //����������
    GPIO_InitStructure.Alternate=GPIO_AF1_TIM2;   //PA5����ΪTIM2ͨ��1
    HAL_GPIO_Init(GPIOA,&GPIO_InitStructure); 
}

/*******************************************************************************
* �� �� ��         : Touch_Get_Val
* ��������		   : ���ز���ߵ�ƽֵ
* ��    ��         : ��
* ��    ��         : ����ߵ�ƽֵ
*******************************************************************************/
u16 Touch_Get_Val(void)
{
	Touch_Reset();
	while(__HAL_TIM_GET_FLAG(&TIM2_Handler,TIM_FLAG_CC1)==RESET) //�ȴ�����������
    {
        if(__HAL_TIM_GET_COUNTER(&TIM2_Handler)>Touch_ARR_MAX_VAL-500) 
			return __HAL_TIM_GET_COUNTER(&TIM2_Handler);//��ʱ�ˣ�ֱ�ӷ���CNT��ֵ
    }
    return HAL_TIM_ReadCapturedValue(&TIM2_Handler,TIM_CHANNEL_1);
}

/*******************************************************************************
* �� �� ��         : Touch_Key_Init
* ��������		   : ����������ʼ��
* ��    ��         : psc:��Ƶϵ��,ԽС,������Խ��
* ��    ��         : 0������
					 1��������
*******************************************************************************/
u8 Touch_Key_Init(u8 psc)  
{
	u8 i;
	u16 buf[10];
	u8 j;
	u16 temp;
	TIM2_CH1_Input_Init(Touch_ARR_MAX_VAL,psc);
	
	for(i=0;i<10;i++) //��ȡ10��Ϊ����ʱ��Ĵ���ֵ
	{
		buf[i]=Touch_Get_Val();
		delay_ms(10);
	}
	
	for(i=0;i<9;i++)   //��С��������
	{
		for(j=i+1;j<10;j++)
		{
			if(buf[i]>buf[j])
			{
				temp=buf[i];
				buf[j]=buf[j];
				buf[j]=temp;
			}
		}
	}
	
	temp=0;
	for(i=2;i<8;i++)  //ȡ�м�6����ֵ��� ȡ��ƽ����
	{
		temp+=buf[i];
	}
	touch_default_val=temp/6;
	printf("touch_default_val=%d \r\n",touch_default_val);
	if(touch_default_val>Touch_ARR_MAX_VAL/2)
	{
		return 1;//��ʼ����������Touch_ARR_MAX_VAL/2����ֵ,������!
	}
	return 0;
}

/*******************************************************************************
* �� �� ��         : Touch_Get_MaxVal
* ��������		   : ��ȡn��,ȡ���ֵ
* ��    ��         : n��������ȡ�Ĵ���
* ��    ��         : n�ζ������������������ֵ
*******************************************************************************/
u16 Touch_Get_MaxVal(u8 n)
{
	u16 temp=0;
	u16 res=0; 
	while(n--)
	{
		temp=Touch_Get_Val();//�õ�һ��ֵ
		if(temp>res)res=temp;
	}
	return res;
}  

/*******************************************************************************
* �� �� ��         : Touch_Key_Scan
* ��������		   : ��������ɨ��
* ��    ��         : 0����֧����������(����һ�α����ɿ����ܰ���һ��)
					 1��֧����������(����һֱ����)
* ��    ��         : 0��û�а���
					 1���а���
*******************************************************************************/										  
#define TOUCH_GATE_VAL 	100	//����������ֵ,Ҳ���Ǳ������tpad_default_val+TOUCH_GATE_VAL,����Ϊ����Ч����.
u8 Touch_Key_Scan(u8 mode)
{
	static u8 keyen=0;	//0,���Կ�ʼ���;>0,�����ܿ�ʼ���	 
	u8 res=0;
	u8 sample=3;		//Ĭ�ϲ�������Ϊ3��	 
	u16 rval;
	if(mode)
	{
		sample=6;	//֧��������ʱ�����ò�������Ϊ6��
		keyen=0;	//֧������	  
	}
	rval=Touch_Get_MaxVal(sample); 
	if(rval>(touch_default_val+TOUCH_GATE_VAL)&&rval<(10*touch_default_val))//����touch_default_val+TPAD_GATE_VAL,��С��10��touch_default_val,����Ч
	{							 
		if((keyen==0)&&(rval>(touch_default_val+TOUCH_GATE_VAL)))	//����touch_default_val+TOUCH_GATE_VAL,��Ч
		{
			res=1;
		}	   
		printf("�����󲶻�ߵ�ƽֵΪ��%d\r\n",rval);		     	    					   
		keyen=3;				//����Ҫ�ٹ�3��֮����ܰ�����Ч   
	} 
	if(keyen)keyen--;		   							   		     	    					   
	return res;
}	 
