﻿// user_A011_E011.h
/*
*版权
*
* 文件名称:user_A011_E011.h
* 文件标识:配置管理计划书
* 摘要:
*
* 当前版本:V1.0
* 作者:刘家辉
* 完成日期:
* 修改者:
* 修改日期:
*
* 取代版本:
* 原作者:刘家辉
* 完成日期:
 */

#ifndef _USER_A011_E011_h
#define _USER_A011_E011_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include<arduino.h>	//引用标准库的头文件
#include"user_initialization.h"//初始化
#include"user_crc8.h"//CRC校验
#include"user_judgement.h"//判断

 //引脚定义



 //全局变量
//static unsigned char Receive_IsBroadcast = 0x00; //是否广播指令

static unsigned char E011[24];//用来存放E011发送出去的数组

static int E011_FrameHead = 0xFE;		//E011的帧头

static int E011_FrameId1 = 0xE0;		//E011的帧ID1
static int E011_FrameId2 = 0x11;		//E011的帧ID2

static int E011_DataLen = 0x0D;			//E011的数据长度

static int E011_DeviceTypeID1 = 0xC0;	//E011的设备类型1
static int E011_DeviceTypeID2 = 0x02;	//E011的设备类型2

static int E011_IsBroadcast = 0x00;		//E011的是否广播指令

static int E011_ZoneId = 0x00;			//E011的区域

static int E011_DeviceSN1 = 0x00;		//E011的SN1
static int E011_DeviceSN2 = 0x00;		//E011的SN2
static int E011_DeviceSN3 = 0x00;		//E011的SN3
static int E011_DeviceSN4 = 0x00;		//E011的SN4
static int E011_DeviceSN5 = 0x00;		//E011的SN5
static int E011_DeviceSN6 = 0x00;		//E011的SN6
static int E011_DeviceSN7 = 0x00;		//E011的SN7
static int E011_DeviceSN8 = 0x00;		//E011的SN8
static int E011_DeviceSN9 = 0x00;		//E011的SN9

static int E011_CRC8 = 0x00;			//E011的CRC8校验码

static int E011_FrameEnd1 = 0x0D;		//E011的帧尾1
static int E011_FrameEnd2 = 0x0A;		//E011的帧尾2
static int E011_FrameEnd3 = 0x0D;		//E011的帧尾3
static int E011_FrameEnd4 = 0x0A;		//E011的帧尾4
static int E011_FrameEnd5 = 0x0D;		//E011的帧尾5
static int E011_FrameEnd6 = 0x0A;		//E011的帧尾6

static unsigned char E011_Check_Data[50];   //用来存放接收到的数据
static int E011_Check_Length = 0;


//全局函数声明
void Receive_A011(unsigned char * Judgement_Data, int Judgement_Length);  //A011函数
unsigned char Send_E011(int Receive_IsBroadcast);  //E011函数
unsigned char E011_init();	//E011初始化函数


//类结构声明



#endif

