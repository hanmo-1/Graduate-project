/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*              操作Wifi功能的头文件               */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __WIFI_H
#define __WIFI_H

#include "usart.h"	    //包含需要的头文件


#define RESET_IO(x)       PIout(11)=x         //PI11管脚控制wifi模块复位引脚
#define WiFi_printf       u3_printf           //串口2控制 WiFi
#define WiFi_RxCounter    Usart3_RxCounter    //串口2控制 WiFi
#define WiFi_RX_BUF       Usart3_RxBuff       //串口2控制 WiFi
#define WiFi_RXBUFF_SIZE  USART3_RXBUFF_SIZE  //串口2控制 WiFi

#define SSID   "mi"                     //路由器SSID名称
#define PASS   "1qaz0plm"                 //路由器密码

void WiFi_ResetIO_Init(void);
char WiFi_SendCmd(char *cmd, int timeout);
char WiFi_Reset(int timeout);
char WiFi_JoinAP(int timeout);
char WiFi_Connect_Server(int timeout);
char WiFi_Smartconfig(int timeout);
char WiFi_WaitAP(int timeout);
char WiFi_Connect_IoTServer(void);


#endif


