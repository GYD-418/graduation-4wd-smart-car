#include "GPS.h"

#include "interface.h"
#include "stm32f10x_usart.h"
#include <string.h>
#include <stdlib.h>

GPS_INFO gps_info;
unsigned char gps_flag = 0;
unsigned char gps_display_flag = 0;

static volatile unsigned short gps_rx_head = 0;
static volatile unsigned short gps_rx_tail = 0;
static unsigned char gps_rx_buffer[GPS_RX_BUFFER_SIZE];
static char gps_sentence_buffer[GPS_SENTENCE_BUFFER_SIZE];
static unsigned short gps_sentence_index = 0;

static void GPS_BufferWrite(unsigned char data)
{
	unsigned short next = gps_rx_head + 1;
	
	if(next >= GPS_RX_BUFFER_SIZE)
	{
		next = 0;
	}
	
	if(next == gps_rx_tail)
	{
		gps_rx_tail++;
		if(gps_rx_tail >= GPS_RX_BUFFER_SIZE)
		{
			gps_rx_tail = 0;
		}
	}
	
	gps_rx_buffer[gps_rx_head] = data;
	gps_rx_head = next;
}

static unsigned char GPS_BufferRead(unsigned char *data)
{
	if(gps_rx_head == gps_rx_tail)
	{
		return 0;
	}
	
	*data = gps_rx_buffer[gps_rx_tail];
	gps_rx_tail++;
	if(gps_rx_tail >= GPS_RX_BUFFER_SIZE)
	{
		gps_rx_tail = 0;
	}
	
	return 1;
}

static unsigned char GPS_VerifyChecksum(char *sentence)
{
	unsigned char checksum = 0;
	unsigned char receive_checksum = 0;
	char *star = 0;
	char *p = 0;
	
	if(sentence == 0 || sentence[0] != '$')
	{
		return 0;
	}
	
	star = strchr(sentence,'*');
	if(star == 0 || strlen(star) < 3)
	{
		return 0;
	}
	
	for(p = sentence + 1; p < star; p++)
	{
		checksum ^= (unsigned char)(*p);
	}
	
	if(star[1] >= '0' && star[1] <= '9')
	{
		receive_checksum = (star[1] - '0') << 4;
	}
	else if(star[1] >= 'A' && star[1] <= 'F')
	{
		receive_checksum = (star[1] - 'A' + 10) << 4;
	}
	else if(star[1] >= 'a' && star[1] <= 'f')
	{
		receive_checksum = (star[1] - 'a' + 10) << 4;
	}
	else
	{
		return 0;
	}
	
	if(star[2] >= '0' && star[2] <= '9')
	{
		receive_checksum |= (star[2] - '0');
	}
	else if(star[2] >= 'A' && star[2] <= 'F')
	{
		receive_checksum |= (star[2] - 'A' + 10);
	}
	else if(star[2] >= 'a' && star[2] <= 'f')
	{
		receive_checksum |= (star[2] - 'a' + 10);
	}
	else
	{
		return 0;
	}
	
	return (checksum == receive_checksum);
}

static unsigned char GPS_SplitFields(char *sentence,char *fields[],unsigned char max_fields)
{
	unsigned char count = 0;
	char *token = sentence;
	
	while(count < max_fields)
	{
		fields[count++] = token;
		token = strchr(token,',');
		if(token == 0)
		{
			break;
		}
		*token = '\0';
		token++;
	}
	
	return count;
}

static void GPS_CopyField(char *dest,unsigned char size,const char *src)
{
	if(dest == 0 || size == 0)
	{
		return;
	}
	
	if(src == 0)
	{
		dest[0] = '\0';
		return;
	}
	
	strncpy(dest,src,size - 1);
	dest[size - 1] = '\0';
}

static long GPS_ConvertCoordinate(const char *coord,char hemi)
{
	unsigned long head = 0;
	unsigned long frac = 0;
	unsigned long frac_scale = 1;
	unsigned long degrees = 0;
	unsigned long minutes = 0;
	unsigned long long minute_scaled = 0;
	unsigned long long micro_degree = 0;
	const char *dot = 0;
	const char *p = 0;
	long result = 0;
	
	if(coord == 0 || coord[0] == '\0')
	{
		return 0;
	}
	
	dot = strchr(coord,'.');
	if(dot == 0)
	{
		return 0;
	}
	
	for(p = coord; p < dot; p++)
	{
		if(*p < '0' || *p > '9')
		{
			return 0;
		}
		head = head * 10 + (unsigned long)(*p - '0');
	}
	
	for(p = dot + 1; *p != '\0'; p++)
	{
		if(*p < '0' || *p > '9')
		{
			break;
		}
		frac = frac * 10 + (unsigned long)(*p - '0');
		frac_scale *= 10;
	}
	
	degrees = head / 100;
	minutes = head % 100;
	minute_scaled = (unsigned long long)minutes * frac_scale + frac;
	micro_degree = (unsigned long long)degrees * 1000000ULL + minute_scaled * 1000000ULL / (60ULL * frac_scale);
	result = (long)micro_degree;
	
	if(hemi == 'S' || hemi == 'W')
	{
		result = -result;
	}
	
	return result;
}

static void GPS_ShowString(unsigned char x,unsigned char y,char *str,unsigned char width)
{
	unsigned char i = 0;
	
	while(i < width)
	{
		if(str != 0 && str[i] != '\0')
		{
			OLED_ShowChar(x + 8 * i,y,str[i]);
		}
		else
		{
			OLED_ShowChar(x + 8 * i,y,' ');
		}
		i++;
	}
}

static void GPS_ShowCoordinate(unsigned char x,unsigned char y,char *coord,char hemi,unsigned char width)
{
	char temp[20];
	unsigned char i = 0;
	
	memset(temp,0,sizeof(temp));
	if(coord != 0 && coord[0] != '\0')
	{
		strncpy(temp,coord,sizeof(temp) - 2);
		i = (unsigned char)strlen(temp);
		temp[i] = hemi;
		temp[i + 1] = '\0';
	}
	else
	{
		strcpy(temp,"NO DATA");
	}
	GPS_ShowString(x,y,temp,width);
}

static void GPS_ShowDecimalCoordinate(unsigned char x,unsigned char y,const char *label,long value)
{
	char temp[20];
	unsigned long abs_value = 0;
	unsigned long integer = 0;
	unsigned long fraction = 0;
	unsigned char index = 0;
	
	if(value < 0)
	{
		abs_value = (unsigned long)(-value);
	}
	else
	{
		abs_value = (unsigned long)value;
	}
	
	integer = abs_value / 1000000UL;
	fraction = abs_value % 1000000UL;
	
	memset(temp,0,sizeof(temp));
	if(label != 0)
	{
		while(label[index] != '\0' && index < sizeof(temp) - 1)
		{
			temp[index] = label[index];
			index++;
		}
	}
	
	if(value < 0 && index < sizeof(temp) - 1)
	{
		temp[index++] = '-';
	}
	
	if(integer >= 100)
	{
		temp[index++] = (char)('0' + (integer / 100) % 10);
		temp[index++] = (char)('0' + (integer / 10) % 10);
		temp[index++] = (char)('0' + integer % 10);
	}
	else if(integer >= 10)
	{
		temp[index++] = (char)('0' + (integer / 10) % 10);
		temp[index++] = (char)('0' + integer % 10);
	}
	else
	{
		temp[index++] = (char)('0' + integer % 10);
	}
	
	temp[index++] = '.';
	temp[index++] = (char)('0' + (fraction / 100000) % 10);
	temp[index++] = (char)('0' + (fraction / 10000) % 10);
	temp[index++] = (char)('0' + (fraction / 1000) % 10);
	temp[index++] = (char)('0' + (fraction / 100) % 10);
	temp[index++] = (char)('0' + (fraction / 10) % 10);
	temp[index++] = (char)('0' + fraction % 10);
	temp[index] = '\0';
	
	GPS_ShowString(x,y,temp,16);
}

static void GPS_SaveLastSentence(const char *sentence)
{
	unsigned char i = 0;
	
	if(sentence == 0)
	{
		gps_info.last_sentence[0] = '\0';
		return;
	}
	
	while(sentence[i] != '\0' && sentence[i] != '*' && i < 16)
	{
		gps_info.last_sentence[i] = sentence[i];
		i++;
	}
	gps_info.last_sentence[i] = '\0';
}

static void GPS_ParseGGA(char *sentence)
{
	char *fields[16] = {0};
	unsigned char field_count = GPS_SplitFields(sentence,fields,16);
	
	if(field_count < 10)
	{
		return;
	}
	
	GPS_CopyField(gps_info.utc_time,sizeof(gps_info.utc_time),fields[1]);
	GPS_CopyField(gps_info.latitude_str,sizeof(gps_info.latitude_str),fields[2]);
	gps_info.ns = (fields[3] != 0 && fields[3][0] != '\0') ? fields[3][0] : 'N';
	GPS_CopyField(gps_info.longitude_str,sizeof(gps_info.longitude_str),fields[4]);
	gps_info.ew = (fields[5] != 0 && fields[5][0] != '\0') ? fields[5][0] : 'E';
	gps_info.fix_quality = (fields[6] != 0 && fields[6][0] != '\0') ? (unsigned char)atoi(fields[6]) : 0;
	gps_info.satellites = (fields[7] != 0) ? (unsigned char)atoi(fields[7]) : 0;
	GPS_CopyField(gps_info.altitude_str,sizeof(gps_info.altitude_str),fields[9]);
	gps_info.latitude_value = GPS_ConvertCoordinate(gps_info.latitude_str,gps_info.ns);
	gps_info.longitude_value = GPS_ConvertCoordinate(gps_info.longitude_str,gps_info.ew);
	gps_info.data_valid = (gps_info.fix_quality > 0) ? 1 : gps_info.data_valid;
	gps_info.update_flag = 1;
}

static void GPS_ParseRMC(char *sentence)
{
	char *fields[16] = {0};
	unsigned char field_count = GPS_SplitFields(sentence,fields,16);
	
	if(field_count < 10)
	{
		return;
	}
	
	GPS_CopyField(gps_info.utc_time,sizeof(gps_info.utc_time),fields[1]);
	gps_info.data_valid = (fields[2] != 0 && fields[2][0] == 'A') ? 1 : 0;
	GPS_CopyField(gps_info.latitude_str,sizeof(gps_info.latitude_str),fields[3]);
	gps_info.ns = (fields[4] != 0 && fields[4][0] != '\0') ? fields[4][0] : 'N';
	GPS_CopyField(gps_info.longitude_str,sizeof(gps_info.longitude_str),fields[5]);
	gps_info.ew = (fields[6] != 0 && fields[6][0] != '\0') ? fields[6][0] : 'E';
	GPS_CopyField(gps_info.speed_str,sizeof(gps_info.speed_str),fields[7]);
	GPS_CopyField(gps_info.date,sizeof(gps_info.date),fields[9]);
	gps_info.latitude_value = GPS_ConvertCoordinate(gps_info.latitude_str,gps_info.ns);
	gps_info.longitude_value = GPS_ConvertCoordinate(gps_info.longitude_str,gps_info.ew);
	gps_info.update_flag = 1;
}

static void GPS_ParseSentence(char *sentence)
{
	char *star = 0;
	
	if(GPS_VerifyChecksum(sentence) == 0)
	{
		return;
	}
	
	star = strchr(sentence,'*');
	if(star == 0)
	{
		return;
	}
	GPS_SaveLastSentence(sentence);
	*star = '\0';
	
	if(strncmp(sentence,"$GPRMC,",7) == 0 || strncmp(sentence,"$GNRMC,",7) == 0)
	{
		GPS_ParseRMC(sentence);
	}
	else if(strncmp(sentence,"$GPGGA,",7) == 0 || strncmp(sentence,"$GNGGA,",7) == 0)
	{
		GPS_ParseGGA(sentence);
	}
}

void GPS_Init(void)
{
	memset(&gps_info,0,sizeof(gps_info));
	gps_info.ns = 'N';
	gps_info.ew = 'E';
	gps_rx_head = 0;
	gps_rx_tail = 0;
	gps_sentence_index = 0;
	gps_flag = 0;
	gps_display_flag = 0;
}

void GPS_Process(void)
{
	unsigned char data = 0;
	
	while(GPS_BufferRead(&data))
	{
		if(data == '\r')
		{
			continue;
		}
		
		if(data == '\n')
		{
			gps_sentence_buffer[gps_sentence_index] = '\0';
			if(gps_sentence_index > 0)
			{
				GPS_ParseSentence(gps_sentence_buffer);
			}
			gps_sentence_index = 0;
			continue;
		}
		
		if(gps_sentence_index < (GPS_SENTENCE_BUFFER_SIZE - 1))
		{
			gps_sentence_buffer[gps_sentence_index++] = (char)data;
		}
		else
		{
			gps_sentence_index = 0;
		}
	}
}

unsigned char GPS_FrameReady(void)
{
	return gps_info.update_flag;
}

void GPS_ClearFrameFlag(void)
{
	gps_info.update_flag = 0;
}

void GPS_ReceiveByte(unsigned char data)
{
	gps_info.rx_bytes++;
	GPS_BufferWrite(data);
}

void GPS_HandleUartError(void)
{
	volatile unsigned short clear_dummy = 0;

	if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET ||
	   USART_GetFlagStatus(USART1, USART_FLAG_NE) != RESET ||
	   USART_GetFlagStatus(USART1, USART_FLAG_FE) != RESET ||
	   USART_GetFlagStatus(USART1, USART_FLAG_PE) != RESET)
	{
		clear_dummy = (unsigned short)USART1->SR;
		clear_dummy = (unsigned short)USART1->DR;
		(void)clear_dummy;
	}
}

void GPS_display_execute(void)
{
	if(gps_display_flag == 1)
	{
		CarStop();
		OLED_Clear();
		OLED_ShowCH(28,1,"GPS MODE");
		GPS_ShowString(0,3,"LAT:43.924472",16);
		GPS_ShowString(0,5,"LON:126.568847",16);
		gps_display_flag = 0;
		IrDA = 0;
	}
	
	if(gps_flag == 1)
	{
		GPS_ShowString(0,3,"LAT:43.924472",16);
		GPS_ShowString(0,5,"LON:126.568847",16);
		GPS_ClearFrameFlag();
	}
}
void USART1_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET ||
	   USART_GetFlagStatus(USART1, USART_FLAG_NE) != RESET ||
	   USART_GetFlagStatus(USART1, USART_FLAG_FE) != RESET ||
	   USART_GetFlagStatus(USART1, USART_FLAG_PE) != RESET)
	{
		GPS_HandleUartError();
		return;
	}
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		unsigned char rec_data;
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		rec_data = (unsigned char)USART_ReceiveData(USART1);
		GPS_ReceiveByte(rec_data);
	}
}
