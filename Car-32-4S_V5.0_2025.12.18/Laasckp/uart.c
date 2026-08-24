#include "uart.h"

unsigned char BT_flag=0;
char BT = 0;
unsigned char USART2_RX_BUF[200];
unsigned char USART2_RX_FLAG=0;
unsigned char USART2_RX_LEN=0;
unsigned char USART2_RX_TEMP;

//USART2 TxD PA2   RxD PA3
void USART2Conf(u32 baudRate)
{
	NVIC_InitTypeDef 	NVIC_InitStruct;
	USART_InitTypeDef USART_InitSturct;
	GPIO_InitTypeDef GPIO_InitStruct;

	//USART2_Tx_Pin PA2
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA , &GPIO_InitStruct);

	//USART2_Rx_Pin PA3
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	USART_InitSturct.USART_BaudRate = baudRate;
	USART_InitSturct.USART_WordLength = USART_WordLength_8b;
	USART_InitSturct.USART_StopBits = USART_StopBits_1;
	USART_InitSturct.USART_Parity = USART_Parity_No;
	USART_InitSturct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitSturct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2 , &USART_InitSturct);
	USART_ITConfig(USART2 , USART_IT_RXNE , ENABLE);
	USART_Cmd(USART2 , ENABLE);
	USART_ClearFlag(USART2 , USART_FLAG_TC);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}

void USART1Conf(u32 baudRate)
{
	NVIC_InitTypeDef 	NVIC_InitStruct;
	USART_InitTypeDef USART_InitSturct;
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA , &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	USART_InitSturct.USART_BaudRate = baudRate;
	USART_InitSturct.USART_WordLength = USART_WordLength_8b;
	USART_InitSturct.USART_StopBits = USART_StopBits_1;
	USART_InitSturct.USART_Parity = USART_Parity_No;
	USART_InitSturct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitSturct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1 , &USART_InitSturct);
	USART_ITConfig(USART1 , USART_IT_RXNE , ENABLE);
	USART_Cmd(USART1 , ENABLE);
	USART_ClearFlag(USART1 , USART_FLAG_TC);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}

void USART3Conf(u32 baudRate)
{
	NVIC_InitTypeDef 	NVIC_InitStruct;
	USART_InitTypeDef USART_InitSturct;
	GPIO_InitTypeDef GPIO_InitStruct;

	//USART3_Tx_Pin PB10
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB , &GPIO_InitStruct);

	//USART3_Rx_Pin PB11
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	USART_InitSturct.USART_BaudRate = baudRate;
	USART_InitSturct.USART_WordLength = USART_WordLength_8b;
	USART_InitSturct.USART_StopBits = USART_StopBits_1;
	USART_InitSturct.USART_Parity = USART_Parity_No;
	USART_InitSturct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitSturct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3 , &USART_InitSturct);
	USART_ITConfig(USART3 , USART_IT_RXNE , ENABLE);
	USART_Cmd(USART3 , ENABLE);
	USART_ClearFlag(USART3 , USART_FLAG_TC);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}

void PutChar(u8 Data)
{
	USART_SendData(USART3 , Data);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
}

void PutStr(char *str)
{
	while(*str != '\0')
	{
		USART_SendData(USART3 , *str++);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
	}
}

void PutNChar(u8 *buf , u16 size)
{
	u8 i;
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
	for(i=0;i<size;i++)
	{
		USART_SendData(USART3 , buf[i]);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
	}
}

void PutStr2(char *str)
{
	while(*str != '\0')
	{
		USART_SendData(USART2 , *str++);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
	}
}

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		USART2_RX_TEMP = USART_ReceiveData(USART2);
		if(USART2_RX_TEMP == '\n')
		{
			USART2_RX_BUF[USART2_RX_LEN] = '\0';
			USART2_RX_FLAG = 1;
		}
		else if(USART2_RX_LEN < sizeof(USART2_RX_BUF) - 1)
		{
			USART2_RX_BUF[USART2_RX_LEN++] = USART2_RX_TEMP;
		}
	}
}

void USART3_IRQHandler(void)
{
	unsigned char rec_data;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		rec_data = USART_ReceiveData(USART3);
		if(rec_data==0x00||rec_data == 0x61||rec_data == 0x62||rec_data == 0x63||rec_data == 0x64||
			 rec_data==0x1C||rec_data == 0x45||rec_data == 0x46||rec_data == 0x47||rec_data == 0x16||
			 rec_data==0x0D||rec_data == 0x44||rec_data == 0x40||rec_data == 0x43||rec_data == 0x07)
		{
			BT_flag = 1;
			IrDA = rec_data;
		}
	}
}
