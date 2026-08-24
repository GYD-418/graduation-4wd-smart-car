#ifndef __UART_H_
#define __UART_H_

#include "interface.h"//ģ��ӿڶ���ͷ�ļ�

extern char BT ;//��������ָ��
extern unsigned char BT_flag;//�������Ʊ�־λ

extern unsigned char USART2_RX_BUF[200];//USART2接收缓冲区
extern unsigned char USART2_RX_FLAG;     //USART2接收完成标志
extern unsigned char USART2_RX_LEN;      //USART2接收数据长度

void USART1Conf(u32 baudRate);
void USART2Conf(u32 baudRate);
void USART3Conf(u32 baudRate);
void PutChar(u8 Data);
void PutNChar(u8 *buf , u16 size);
void PutStr(char *str);
void PutStr2(char *str);

#endif
