/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*         实现DHT11采集温湿度功能的头文件         */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __DHT11_H
#define __DHT11_H 
 

#define DHT11_IO_IN()  {GPIOA->CRH&=0XFFFFFFF0;GPIOA->CRH|=8<<0;}       //PA8控制DHT11，设置为输出
#define DHT11_IO_OUT() {GPIOA->CRH&=0XFFFFFFF0;GPIOA->CRH|=3<<0;}       //PA8控制DHT11，设置为输入
										   
#define DHT11_OUT(x)    GPIO_WriteBit(GPIOA, GPIO_Pin_8, (BitAction)x)  //PA8控制DHT11
#define DHT11_DQ_IN     GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)        //PA8控制DHT11	
   
void DHT11_Rst(void);
char DHT11_Check(void);
char DHT11_Read_Bit(void);
char DHT11_Read_Byte(void);
char DHT11_Read_Data(char *temp, char *humi);
char DHT11_Init(void);

#endif















