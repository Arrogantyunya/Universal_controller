// user_crc8.h
/*
*��Ȩ
*
* �ļ�����:user_crc8.h
* �ļ���ʶ:���ù���ƻ���
* ժҪ:crc8У��ֵ�ļ���Ա�
*
* ��ǰ�汾:V1.0
* ����:���һ�
* �������:
* �޸���:
* �޸�����:
*
* ȡ���汾:
* ԭ����:���һ�
* �������:
 */


#ifndef _USER_CRC8_h
#define _USER_CRC8_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

//#include <arduino.h>	//���ñ�׼���ͷ�ļ�


#define AL2_FCS_COEF ((1 << 7) + (1 << 6) + (1 << 5))


typedef unsigned long  DWORD;
typedef unsigned short WORD;

static unsigned char Check_Data[50];   //������Ž��յ�������
static int Check_Length = 0;


//ȫ�ֺ�������
unsigned char GetCrc8(unsigned char * Check_Data, int Check_Length);

//��ṹ����



#endif

