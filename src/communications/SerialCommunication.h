/*
 * SerialCommunication.h
 *
 *  Created on: Aug 2, 2017
 *      Author: wlc
 */

#ifndef SERIALCOMMUNICATION_H_
#define SERIALCOMMUNICATION_H_
//串口相关的头文件
#include<stdio.h>      /*标准输入输出定义*/
#include<stdlib.h>     /*标准函数库定义*/
#include<unistd.h>     /*Unix 标准函数定义*/
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>      /*文件控制定义*/
#include<termios.h>    /*PPSIX 终端控制定义*/
#include<errno.h>      /*错误号定义*/
#include<string.h>
#include "messagee/Message.h"
#include "messagee/TrackingMessage.h"
#include "messagee/Fpdcmessage.h"
#include "../tools/Configuration.h"
#include "CommunicationChannel.h"
#include "../algorithm/Algorithm.h"
//宏定义

namespace va{
class SerialCommunication : public Algorithm{
public:
	SerialCommunication(string anaName, string loggerName);
	virtual ~SerialCommunication();
	bool processMessage(Message* msg);
	int UART0_Open(int fd,char* port);
	void UART0_Close(int fd);
	int UART0_Set(int fd,int speed,int flow_ctrl,int databits,int stopbits,int parity);
	int UART0_Init(int fd, int speed,int flow_ctrl,int databits,int stopbits,int parity);
	Message* UART0_Recv(int fd, char *rcv_buf,int data_len);
	int UART0_Send(int fd, Message* msg);
	void run();
	void setFd(int fd);
	int fd=-5;
int len =0;
};
}
#endif /* SERIALCOMMUNICATION_H_ */
