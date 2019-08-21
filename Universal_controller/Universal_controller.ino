﻿// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       Universal_controller.ino
    Created:	2019/7/22 星期一 21:40:55
    Author:     刘家辉
*/

#include "user_Set_Correlation.h"
#include "user_A025_E025.h"
#include "user_A021_E021.h"
#include "user_crc8.h"
#include "user_judgement.h"
#include "user_A011_E011.h"
#include <arduino.h>
#include "user_lorainit.h"
#include "user_initialization.h"
#include "AT24CXX.h"
#include "i2c.h"


//----------------------------
static String LORA_RecData1, LORA_RecData2;
//全局变量
static unsigned char Receive_Data[200];//用来存放接收到的数据
static int Receive_Length = 0;//接收数据的长度
static int CRC_Check_num = 0x00;//CRC校验的数值


//----------------------------
void LORA_Receive_information(void);


//函 数 名：setup() 
//功能描述：起始代码
//函数说明：
//调用函数：
//全局变量：
//输 入：
//返 回：
/////////////////////////////////////////////////////////////////////
void setup()
{
	Initialization();//初始化函数
	Serial.println("初始化执行结束");
	if (AT24CXX_ReadOneByte(0) == 0x01 && AT24CXX_ReadOneByte(1) == 0x01)
	{
		Serial.println("初始化程序执行成功！");
		//初始化完成状态灯1红绿交替闪烁5次
		for (size_t i = 0; i < 5; i++)
		{
			digitalWrite(LED1, HIGH);
			digitalWrite(LED2, LOW);
			delay(200);
			digitalWrite(LED1, LOW);
			digitalWrite(LED2, HIGH);
			delay(200);
			digitalWrite(LED1, LOW);
			digitalWrite(LED2, LOW);
		}

		//while (AT24CXX_ReadOneByte(2) == 0x00)//Register_OK_flag	已经完成申号的标志位
		//{
		//	//代表未设置工作参数
		//	Serial.println("未设置工作参数,如需要设置工作参数，请长按按键1");
		//	digitalWrite(LED2, HIGH);
		//	delay(1500);
		//	//等待按键1按下
		//	if (digitalRead(K1) == LOW)
		//	{
		//		delay(2000);
		//		if (digitalRead(K1) == LOW)
		//		{
		//			digitalWrite(LED2, LOW);
		//			Serial.println("K1按下");
		//			Serial.println("本设备开始上报当前的设置参数");

		//			delay(250);
		//			//进入E011函数上报请求当前参数
		//			Send_E011(Receive_IsBroadcast);//这里的Receive_IsBroadcast是否有值？
		//			AT24CXX_WriteOneByte(2, 0X01);
		//		}
		//	}
		//}
	}
	else
	{
		while (1)
		{
			digitalWrite(LED1, HIGH);
			Serial.println("初始化程序执行失败");
			delay(2000);
		}
	}
	
}

//函 数 名：loop() 
//功能描述：循环执行的代码
//函数说明：循环执行的代码
//调用函数：Receive_information()、forswitch()、Initialization()、setup()
//全局变量：
//输 入：
//返 回：
/////////////////////////////////////////////////////////////////////
// Add the main program code into the continuous loop() function
void loop()
{
	//读取设置的自动策略
	if (AT24CXX_ReadOneByte(13) == 0x01)
	{
		
	}

	forswitch();

	LORA_Receive_information();	//LORA的接收函数

	if (debug == 1)
	{
		/*Serial.println(String("Delivery_time = ") + Delivery_time);
		delay(1500);*/
	}

	if (millis() - Get_Delivery_oldtime() >= Delivery_time * 1000 && Get_Delivery_oldtime() > 0)
	{
		//进行状态的回执
		Send_E021(Receive_IsBroadcast);
	}

	//这是恢复为出厂设置，请慎用
	if (digitalRead(K1) == LOW)
	{
		delay(2000);
		if (digitalRead(K1) == LOW)
		{
			//-------------------------------------------
			//======测试时所用代码块，实际使用请注释=====
			//将所有的标志位都清为0
			AT24CXX_WriteOneByte(0, 0x00);//lora初始化的标志位
			AT24CXX_WriteOneByte(1, 0x00);//EEPROM设置的标志位
			AT24CXX_WriteOneByte(2, 0x00);//申号的标志位
			AT24CXX_WriteOneByte(13, 0x00);//自动策略的标志位
			//-------------------------------------------
			Serial.println("开始进行恢复出厂设置");

			Initialization();//重新进行初始化的设置
			setup();//重新进入setup()进行申号
		}
	}
	//这是强制启动继电器
	if (digitalRead(K2) == LOW)
	{
		delay(500);
		if (digitalRead(K2) == LOW)
		{
			digitalWrite(KCZJ1, LOW);
			digitalWrite(KCZJ2, LOW);
			/*Serial2.write(Positive_rotation, 8);
			delay(3000);
			Serial2.write(Shutdown, 8);*/
		}
		else
		{
			digitalWrite(KCZJ1, HIGH);
			digitalWrite(KCZJ2, HIGH);
		}
	}
}


//函 数 名：LORA_Receive_information() 
//功能描述：负责接收LORA接收到的信息然后传给Judgement_function()
//函数说明：
//调用函数：Judgement_function()
//全局变量：
//输 入：
//返 回：
/////////////////////////////////////////////////////////////////////
void LORA_Receive_information(void)
{
	//do {
	//		while (Serial3.available() > 0)//接收串口收到的数据
	//		{
	//			if (Receive_Length >= 200) break;
	//			Receive_Data[Receive_Length++] = Serial3.read();
	//			Serial.print(Receive_Data[Receive_Length - 1], HEX);
	//			Serial.print(" ");
	//			delay(5);
	//		}
	//	if (Receive_Length >= 60) Receive_Length = 0;

	//} while (1);
	Receive_Length = 0;
	while (Serial3.available() > 0)//接收串口收到的数据
	{
		if (Receive_Length >= 200) break;
		Receive_Data[Receive_Length++] = Serial3.read();
		/*Serial.print(Receive_Data[Receive_Length - 1], HEX);
		Serial.print(" ");*/
		delay(3);
	}
	if (Receive_Length > 0)
	{
		//Serial.write(Receive_Data,Receive_Length);//发16进制，自动转成ASCII码
		Judgement_Length = 0;//收到新消息清空判断数组的长度值
		Check_Length = 0;//收到新消息清空校验数组的长度值
		Receive_data_lamp();
		if (debug == 1)
		{
			Serial.print("Receive_Length = ");//输出接收到的数据长度
			Serial.println(Receive_Length);
		}

		//--------------------------------------------------------
		//将Receive_Data的值处理得到judgement_Data
		for (int i = 0; i < Receive_Length; i++)
		{
			Judgement_Length = i;
			Judgement_Data[Judgement_Length] = Receive_Data[i];

			//====================================================
			if (debug == 1)
			{
				Serial.print("Judgement_Data ");
				Serial.print(Judgement_Length);
				Serial.print(" :");
				Serial.println(Judgement_Data[Judgement_Length], HEX);
			}
			delay(5);
			//====================================================

		}
		if (debug == 1)
		{
			Serial.print("Judgement_Length = ");
			Serial.println(Judgement_Length);
		}
		//--------------------------------------------------------


		//--------------------------------------------------------
		//将Receive_Data的值处理得到Check_Data
		for (int i = 4; i < Receive_Length - 0x07; i++)
		{
			Check_Data[Check_Length] = Receive_Data[i];
			if (debug == 1)
			{
				Serial.print("Check_Data ");
				Serial.print(Check_Length);
				Serial.print(" :");
				Serial.println(Check_Data[Check_Length], HEX);
			}
			Check_Length++;
			delay(5);
		}
		Serial.print("Check_Length = ");
		Serial.println(Check_Length);
		//--------------------------------------------------------

		if (Check_Length > 0)
		{
			CRC_Check_num = GetCrc8(Check_Data, Check_Length);
			if (debug == 1)
			{
				Serial.print("CRC8计算的值CRC_Check_num为：0x");
				Serial.println(CRC_Check_num, HEX);
			}
		}
		Receive_Length = 0;	//将Receive_Length清零
		Check_Length = 0;	//将Check_Length清零

		//--------------------------------------------------------
		if (debug == 1)
		{
			CRC_Check_num = 0xD6;//这行代码是为了不让我每次都计算CRC，实际使用请注释
		}
		//--------------------------------------------------------

		Judgement_function(Judgement_Data, Judgement_Length, CRC_Check_num);//判断函数
	}


	//-----------------------------------------------------------------------------------------------------
	//if (Serial3.available())
	//{
	//	LORA_RecData1 = Serial3.readString();
	//}
	//if (LORA_RecData1.length() > 0)
	//{
	//	LORA_RecData2 = LORA_RecData1;
	//	Receive_data_lamp();
	//	// void remove (index,count)
	//	//index为必选参数，表示从哪里开始删除，只填这一个参数，会将字符串从索引值开始一直到结尾的所有字符删除
	//	//第二个参数count表示删除多少个字符
	//	//作用到字符串的本身
	//	LORA_RecData1.remove(0, LORA_RecData1.length());
	//	Judgement_Length = 0;//收到新消息清空判断数组的长度值
	//	Check_Length = 0;//收到新消息清空校验数组的长度值

	//	char const *c = LORA_RecData2.c_str();

	//	if (debug == 1)
	//	{
	//		Serial.println(String("LORA_RecData2.length()= ") + String(LORA_RecData2.length()));
	//		Serial.println("接收到的数据为：");
	//		//Serial.write(c);
	//		//int shuzu_length = LORA_RecData2.length();
	//		byte shuzu[50];
	//		LORA_RecData2.getBytes(shuzu, (LORA_RecData2.length()+1));
	//		//LORA_RecData2.toCharArray(shuzu, (LORA_RecData2.length() + 1));
	//		for (size_t i = 0; i < LORA_RecData2.length(); i++)
	//		{
	//			Serial.print(String(i) + String(" / "));
	//			Serial.println(shuzu[i]);
	//		}
	//	}
	//}
}

/***
 *
 *
 *          .,:,,,                                        .::,,,::.
 *        .::::,,;;,                                  .,;;:,,....:i:
 *        :i,.::::,;i:.      ....,,:::::::::,....   .;i:,.  ......;i.
 *        :;..:::;::::i;,,:::;:,,,,,,,,,,..,.,,:::iri:. .,:irsr:,.;i.
 *        ;;..,::::;;;;ri,,,.                    ..,,:;s1s1ssrr;,.;r,
 *        :;. ,::;ii;:,     . ...................     .;iirri;;;,,;i,
 *        ,i. .;ri:.   ... ............................  .,,:;:,,,;i:
 *        :s,.;r:... ....................................... .::;::s;
 *        ,1r::. .............,,,.,,:,,........................,;iir;
 *        ,s;...........     ..::.,;:,,.          ...............,;1s
 *       :i,..,.              .,:,,::,.          .......... .......;1,
 *      ir,....:rrssr;:,       ,,.,::.     .r5S9989398G95hr;. ....,.:s,
 *     ;r,..,s9855513XHAG3i   .,,,,,,,.  ,S931,.,,.;s;s&BHHA8s.,..,..:r:
 *    :r;..rGGh,  :SAG;;G@BS:.,,,,,,,,,.r83:      hHH1sXMBHHHM3..,,,,.ir.
 *   ,si,.1GS,   sBMAAX&MBMB5,,,,,,:,,.:&8       3@HXHBMBHBBH#X,.,,,,,,rr
 *   ;1:,,SH:   .A@&&B#&8H#BS,,,,,,,,,.,5XS,     3@MHABM&59M#As..,,,,:,is,
 *  .rr,,,;9&1   hBHHBB&8AMGr,,,,,,,,,,,:h&&9s;   r9&BMHBHMB9:  . .,,,,;ri.
 *  :1:....:5&XSi;r8BMBHHA9r:,......,,,,:ii19GG88899XHHH&GSr.      ...,:rs.
 *  ;s.     .:sS8G8GG889hi.        ....,,:;:,.:irssrriii:,.        ...,,i1,
 *  ;1,         ..,....,,isssi;,        .,,.                      ....,.i1,
 *  ;h:               i9HHBMBBHAX9:         .                     ...,,,rs,
 *  ,1i..            :A#MBBBBMHB##s                             ....,,,;si.
 *  .r1,..        ,..;3BMBBBHBB#Bh.     ..                    ....,,,,,i1;
 *   :h;..       .,..;,1XBMMMMBXs,.,, .. :: ,.               ....,,,,,,ss.
 *    ih: ..    .;;;, ;;:s58A3i,..    ,. ,.:,,.             ...,,,,,:,s1,
 *    .s1,....   .,;sh,  ,iSAXs;.    ,.  ,,.i85            ...,,,,,,:i1;
 *     .rh: ...     rXG9XBBM#M#MHAX3hss13&&HHXr         .....,,,,,,,ih;
 *      .s5: .....    i598X&&A&AAAAAA&XG851r:       ........,,,,:,,sh;
 *      . ihr, ...  .         ..                    ........,,,,,;11:.
 *         ,s1i. ...  ..,,,..,,,.,,.,,.,..       ........,,.,,.;s5i.
 *          .:s1r,......................       ..............;shs,
 *          . .:shr:.  ....                 ..............,ishs.
 *              .,issr;,... ...........................,is1s;.
 *                 .,is1si;:,....................,:;ir1sr;,
 *                    ..:isssssrrii;::::::;;iirsssssr;:..
 *                         .,::iiirsssssssssrri;;:.
 *
 *
 *
 */