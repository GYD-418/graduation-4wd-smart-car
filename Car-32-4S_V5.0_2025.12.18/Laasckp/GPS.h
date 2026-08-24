#ifndef __GPS_H_
#define __GPS_H_

#include "stm32f10x.h"

#define GPS_RX_BUFFER_SIZE        512
#define GPS_SENTENCE_BUFFER_SIZE  128

typedef struct
{
	unsigned char update_flag;
	unsigned char data_valid;
	unsigned char fix_quality;
	unsigned char satellites;
	char utc_time[11];
	char date[7];
	char ns;
	char ew;
	char latitude_str[16];
	char longitude_str[16];
	char altitude_str[12];
	char speed_str[12];
	long latitude_value;
	long longitude_value;
	unsigned long rx_bytes;
	char last_sentence[17];
}GPS_INFO;

extern GPS_INFO gps_info;
extern unsigned char gps_flag;
extern unsigned char gps_display_flag;

void GPS_Init(void);
void GPS_Process(void);
unsigned char GPS_FrameReady(void);
void GPS_ClearFrameFlag(void);
void GPS_ReceiveByte(unsigned char data);
void GPS_HandleUartError(void);
void GPS_display_execute(void);

#endif
