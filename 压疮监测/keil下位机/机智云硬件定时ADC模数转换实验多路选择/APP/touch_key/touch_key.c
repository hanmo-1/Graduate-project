#include "touch_key.h"
#include "SysTick.h"
#include "usart.h"


TIM_HandleTypeDef TIM2_Handler;         //定时器2句柄 

#define Touch_ARR_MAX_VAL 0xffffffff  //最大的ARR值(TIM2是32位定时器)	
u16 touch_default_val=0;  //为按下触摸按键时的值

/*******************************************************************************
* 函 数 名         : TIM2_CH1_Input_Init
* 函数功能		   : TIM2_CH1输入捕获初始化函数
* 输    入         : arr：自动重装载值
					 psc：预分频系数
* 输    出         : 无
*******************************************************************************/
void TIM2_CH1_Input_Init(u32 arr,u16 psc)
{
	TIM_IC_InitTypeDef TIM2_CH1Config;  
    
    TIM2_Handler.Instance=TIM2;                          //通用定时器3
    TIM2_Handler.Init.Prescaler=psc;                     //分频
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM2_Handler.Init.Period=arr;                        //自动装载值
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_IC_Init(&TIM2_Handler);
    
    TIM2_CH1Config.ICPolarity=TIM_ICPOLARITY_RISING;    //上升沿捕获
    TIM2_CH1Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//映射到TI1上
    TIM2_CH1Config.ICPrescaler=TIM_ICPSC_DIV1;          //配置输入分频，不分频
    TIM2_CH1Config.ICFilter=0;                          //配置输入滤波器，不滤波
    HAL_TIM_IC_ConfigChannel(&TIM2_Handler,&TIM2_CH1Config,TIM_CHANNEL_1);//配置TIM2通道1
    HAL_TIM_IC_Start(&TIM2_Handler,TIM_CHANNEL_1);      //开始捕获TIM2的通道1
}

//定时器2底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_IC_Init()调用
//htim:定时器2句柄
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM2时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
	
    GPIO_Initure.Pin=GPIO_PIN_5;            //PA5
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //推挽复用
    GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    GPIO_Initure.Alternate=GPIO_AF1_TIM2;   //PA5复用为TIM2通道1
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}

/*******************************************************************************
* 函 数 名         : Touch_Reset
* 函数功能		   : 触摸按键复位 先放电然后充电并释放计时器内的值
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Touch_Reset(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.Pin=GPIO_PIN_5;            //PA5
    GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_InitStructure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_InitStructure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);	//PA5输出0，放电

	delay_ms(5);
	__HAL_TIM_CLEAR_FLAG(&TIM2_Handler,TIM_FLAG_CC1|TIM_FLAG_UPDATE);   //清除标志位
    __HAL_TIM_SET_COUNTER(&TIM2_Handler,0); //计数器值归0
    
    GPIO_InitStructure.Mode=GPIO_MODE_AF_PP;      //推挽复用
    GPIO_InitStructure.Pull=GPIO_NOPULL;          //不带上下拉
    GPIO_InitStructure.Alternate=GPIO_AF1_TIM2;   //PA5复用为TIM2通道1
    HAL_GPIO_Init(GPIOA,&GPIO_InitStructure); 
}

/*******************************************************************************
* 函 数 名         : Touch_Get_Val
* 函数功能		   : 返回捕获高电平值
* 输    入         : 无
* 输    出         : 捕获高电平值
*******************************************************************************/
u16 Touch_Get_Val(void)
{
	Touch_Reset();
	while(__HAL_TIM_GET_FLAG(&TIM2_Handler,TIM_FLAG_CC1)==RESET) //等待捕获上升沿
    {
        if(__HAL_TIM_GET_COUNTER(&TIM2_Handler)>Touch_ARR_MAX_VAL-500) 
			return __HAL_TIM_GET_COUNTER(&TIM2_Handler);//超时了，直接返回CNT的值
    }
    return HAL_TIM_ReadCapturedValue(&TIM2_Handler,TIM_CHANNEL_1);
}

/*******************************************************************************
* 函 数 名         : Touch_Key_Init
* 函数功能		   : 触摸按键初始化
* 输    入         : psc:分频系数,越小,灵敏度越高
* 输    出         : 0：正常
					 1：不正常
*******************************************************************************/
u8 Touch_Key_Init(u8 psc)  
{
	u8 i;
	u16 buf[10];
	u8 j;
	u16 temp;
	TIM2_CH1_Input_Init(Touch_ARR_MAX_VAL,psc);
	
	for(i=0;i<10;i++) //读取10次为按下时候的触摸值
	{
		buf[i]=Touch_Get_Val();
		delay_ms(10);
	}
	
	for(i=0;i<9;i++)   //从小到大排序
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
	for(i=2;i<8;i++)  //取中间6个数值求和 取其平均数
	{
		temp+=buf[i];
	}
	touch_default_val=temp/6;
	printf("touch_default_val=%d \r\n",touch_default_val);
	if(touch_default_val>Touch_ARR_MAX_VAL/2)
	{
		return 1;//初始化遇到超过Touch_ARR_MAX_VAL/2的数值,不正常!
	}
	return 0;
}

/*******************************************************************************
* 函 数 名         : Touch_Get_MaxVal
* 函数功能		   : 读取n次,取最大值
* 输    入         : n：连续获取的次数
* 输    出         : n次读数里面读到的最大读数值
*******************************************************************************/
u16 Touch_Get_MaxVal(u8 n)
{
	u16 temp=0;
	u16 res=0; 
	while(n--)
	{
		temp=Touch_Get_Val();//得到一次值
		if(temp>res)res=temp;
	}
	return res;
}  

/*******************************************************************************
* 函 数 名         : Touch_Key_Scan
* 函数功能		   : 触摸按键扫描
* 输    入         : 0：不支持连续触发(按下一次必须松开才能按下一次)
					 1：支持连续触发(可以一直按下)
* 输    出         : 0：没有按下
					 1：有按下
*******************************************************************************/										  
#define TOUCH_GATE_VAL 	100	//触摸的门限值,也就是必须大于tpad_default_val+TOUCH_GATE_VAL,才认为是有效触摸.
u8 Touch_Key_Scan(u8 mode)
{
	static u8 keyen=0;	//0,可以开始检测;>0,还不能开始检测	 
	u8 res=0;
	u8 sample=3;		//默认采样次数为3次	 
	u16 rval;
	if(mode)
	{
		sample=6;	//支持连按的时候，设置采样次数为6次
		keyen=0;	//支持连按	  
	}
	rval=Touch_Get_MaxVal(sample); 
	if(rval>(touch_default_val+TOUCH_GATE_VAL)&&rval<(10*touch_default_val))//大于touch_default_val+TPAD_GATE_VAL,且小于10倍touch_default_val,则有效
	{							 
		if((keyen==0)&&(rval>(touch_default_val+TOUCH_GATE_VAL)))	//大于touch_default_val+TOUCH_GATE_VAL,有效
		{
			res=1;
		}	   
		printf("触摸后捕获高电平值为：%d\r\n",rval);		     	    					   
		keyen=3;				//至少要再过3次之后才能按键有效   
	} 
	if(keyen)keyen--;		   							   		     	    					   
	return res;
}	 
