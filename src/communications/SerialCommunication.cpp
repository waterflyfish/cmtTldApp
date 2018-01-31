/*
 * SerialCommunication.cpp
 *
 *  Created on: Aug 2, 2017
 *      Author: wlc
 */

#include "SerialCommunication.h"
#include <sys/types.h>
#include <sys/stat.h>
/* 鲍率设定被定义在 , 这在 被引入 */
#define BAUDRATE B115200   
/* 定义正确的序列埠 */
#define MODEMDEVICE "/dev/ttyS0"
#define _POSIX_SOURCE 1 /* POSIX 系统兼容 */
 
#define FALSE 0
#define TRUE 1
 

namespace va{
SerialCommunication::SerialCommunication(string anaName, string loggerName) :
				Algorithm(anaName, loggerName){
	// TODO Auto-generated constructor stub

}

SerialCommunication::~SerialCommunication() {
	// TODO Auto-generated destructor stub
}
int SerialCommunication::UART0_Open(int fd,char* port){
	fd = open( port, O_RDWR|O_NOCTTY|O_NDELAY);
	         if (-1 == fd)
	                {
	                       perror("Can't Open Serial Port");
	                       return(-1);
	                }
	     //恢复串口为阻塞状态
	     if(fcntl(fd, F_SETFL, 0) < 0)
	                {
	                       printf("fcntl failed!\n");
	                     return(-1);
	                }
	         else
	                {
	                  printf("fcntl=%d\n",fcntl(fd, F_SETFL,0));
	                }
	      //测试是否为终端设备
	      if(0 == isatty(STDIN_FILENO))
	                {
	                       printf("standard input is not a terminal device\n");
	                  return(-1);
	                }
	  else
	                {
	                     printf("isatty success!\n");
	                }
	  printf("fd->open=%d\n",fd);
          
	  return fd;
}
void  SerialCommunication::UART0_Close(int fd)
{
    close(fd);
}
int SerialCommunication::UART0_Set(int fd,int speed,int flow_ctrl,int databits,int stopbits,int parity)
{

      int   i;
         int   status;
         int   speed_arr[] = { B115200, B19200, B9600, B4800, B2400, B1200, B300};
     int   name_arr[] = {115200,  19200,  9600,  4800,  2400,  1200,  300};

    struct termios options;

    /*tcgetattr(fd,&options)得到与fd指向对象的相关参数，并将它们保存于options,该函数还可以测试配置是否正确，该串口是否可用等。若调用成功，函数返回值为0，若调用失败，函数返回值为1.
    */
    if  ( tcgetattr( fd,&options)  !=  0)
       {
          perror("SetupSerial 1");
          return(-1);
       }

    //设置串口输入波特率和输出波特率
    for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++)
                {
                     if  (speed == name_arr[i])
                            {
                                 cfsetispeed(&options, speed_arr[i]);
                                 cfsetospeed(&options, speed_arr[i]);
                            }
              }

    //修改控制模式，保string anaName, string loggerName证程序不会占用串口
    options.c_cflag |= (CLOCAL|CREAD);
    //修改控制模式，使得能够从串口中读取输入数据
    //options.c_cflag |= CREAD;

    //设置数据流控制
    switch(flow_ctrl)
    {

       case 0 ://不使用流控制
              options.c_cflag &= ~CRTSCTS;
              break;

       case 1 ://使用硬件流控制
              options.c_cflag |= CRTSCTS;
              break;
       case 2 ://使用软件流控制
              options.c_cflag |= IXON | IXOFF | IXANY;
              break;
    }
    //设置数据位
    //屏蔽其他标志位
    options.c_cflag &= ~CSIZE;
    switch (databits)
    {
       case 5    :
                     options.c_cflag |= CS5;
                     break;
       case 6    :
                     options.c_cflag |= CS6;
                     break;
       case 7    :
                 options.c_cflag |= CS7;
                 break;
       case 8:
                 options.c_cflag |= CS8;
                 break;
       default:
                 fprintf(stderr,"Unsupported data size\n");
                 return (-1);
    }
    //设置校验位
    switch (parity)
    {
       case 'n':
       case 'N': //无奇偶校验位。
                 options.c_cflag &= ~PARENB;
                 options.c_iflag &= ~INPCK;
                 break;
       case 'o':
       case 'O'://设置为奇校验
                 options.c_cflag |= (PARODD | PARENB);
                 options.c_iflag |= INPCK;
                 break;
       case 'e':
       case 'E'://设置为偶校验
                 options.c_cflag |= PARENB;
                 options.c_cflag &= ~PARODD;
                 options.c_iflag |= INPCK;
                 break;
       case 's':
       case 'S': //设置为空格
                 options.c_cflag &= ~PARENB;
                 options.c_cflag &= ~CSTOPB;
                 break;
        default:
                 fprintf(stderr,"Unsupported parity\n");
                 return (-1);
    }
    // 设置停止位
    switch (stopbits)
    {
       case 1:
                 options.c_cflag &= ~CSTOPB; break;
       case 2:
                 options.c_cflag |= CSTOPB; break;
       default:
                       fprintf(stderr,"Unsupported stop bits\n");
                       return (-1);
    }

  //修改输出模式，原始数据输出
  options.c_oflag &= ~OPOST;
  
  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);//我加的
//options.c_lflag &= ~(ISIG | ICANON);
options.c_iflag &=~(ICRNL | IGNCR);
options.c_oflag &=~(ONLCR | OCRNL);
options.c_iflag &=~(IXON | IXOFF | IXANY);
    //设置等待时间和最小接收字符
    options.c_cc[VTIME] = 0; /* 读取一个字符等待1*(1/10)s */
    options.c_cc[VMIN] = 1; /* 读取字符的最少个数为1 */

    //如果发生数据溢出，接收数据，但是不再读取 刷新收到的数据但是不读
    tcflush(fd,TCIFLUSH);

    //激活配置 (将修改后的termios数据设置到串口中）
    if (tcsetattr(fd,TCSANOW,&options) != 0)
           {
               perror("com set error!\n");
              return (-1);
           }
    return (0);
}
int SerialCommunication::UART0_Init(int fd, int speed,int flow_ctrl,int databits,int stopbits,int parity)
{
    int err;
    //设置串口数据帧格式
    if (UART0_Set(fd,115200,0,8,1,'N') == -1)
       {
        return -1;
       }
    else
       {
               return  0;
        }
tcflush(fd,TCIOFLUSH);
}
Message* SerialCommunication::UART0_Recv(int fd, char *recv_data,int data_len)
{
    int bytes_read,fs_sel;
    fd_set fs_read;
    mavlink_message_t message;
    	mavlink_status_t status;
    	unsigned int temp = 0;
    struct timeval time;

    //FD_ZERO(&fs_read);
    //FD_SET(fd,&fs_read);

    time.tv_sec = 10;
    time.tv_usec = 0;
    Message* msg;
    int n=0;
char* buf = recv_data;
    //使用select实现串口的多路通信
   //fs_sel = select(fd+1,&fs_read,NULL,NULL,&time);
    // printf("%d GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG\n",fs_sel);
   //if(fs_sel)
      // {
        // bzero(recv_data,sizeof(recv_data));
    	 // while(bytes_read = read(fd,recv_data,data_len)>0){
           
           bytes_read = read(fd,recv_data,255);
//usleep(10);
printf("%d GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG\n",bytes_read);
       printf("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWw\n");
if(bytes_read==21){
//std::string string = std::string(recv_data);
       printf("Datagram is: ");
         
          	for (int i = 0; i < bytes_read; ++i) {
          		temp = recv_data[i];
         
          		printf("%02x ", (unsigned char) temp);
                        recv_data[i] = ((recv_data[i]<<4)&0xF0)|((recv_data[i]>>4)&0x0F);
if (mavlink_parse_char(MAVLINK_COMM_0, recv_data[i], &message, &status)) {

			printf("\nReceived message with ID %d, sequence: %d from component %d of system %d\n", message.msgid, message.seq,
					message.compid, message.sysid);

		}
}
 
          	
	switch (message.msgid) {
	case MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START: {
		TrackingMessage* cMsg = new TrackingMessage(to_string(MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START), LOG_MSG,
				Configuration::getInstance().getPipelineControllerId());
		cMsg->setupFromMavlinkMsg(message);
		msg = dynamic_cast<Message*>(cMsg);
handleMessage(msg);
	}
		break;
	case MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP: {
		TrackingMessage* cMsg = new TrackingMessage(to_string(MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP), LOG_MSG,
				Configuration::getInstance().getPipelineControllerId());
		cMsg->setupFromMavlinkMsg(message);
		msg = dynamic_cast<Message*>(cMsg);
handleMessage(msg);
	}
		break;

	case MAVLINK_MSG_ID_TX1_DEBUG_REQUEST_FP_TRACKED: {
		Fpdcmessage* cMsg = new Fpdcmessage(
				to_string(MAVLINK_MSG_ID_TX1_DEBUG_REQUEST_FP_TRACKED), LOG_MSG, Configuration::getInstance().getPipelineControllerId());
		cMsg->setupFromMavlinkMsg(message);
		msg = dynamic_cast<Message*>(cMsg);
handleMessage(msg);
	}
		break;

	default: {

	}
		break;
	}

}
	return msg;
            //  return len;
      // }
  // else
      // {
      //    printf("Sorry,I am wrong!");
      //        return msg;
      // }
}
int SerialCommunication::UART0_Send(int fd,Message* msg)
{
    mavlink_message_t msgToSend = msg->getMavMsg(Configuration::getInstance().getMavlinkSystemId(), Configuration::getInstance().getMavlinkComponentId(),235, 345);
    				uint8_t buf[MAV_BUFFER_LENGTH];
    				uint16_t len = mavlink_msg_to_send_buffer(buf, &msgToSend);
    				char* reBuf = new char[len];
    				for (int i = 0; i < len; ++i) {
    					//reBuf[i] = buf[i];
                                          reBuf[i] = ((buf[i]<<4)&0xF0)|((buf[i]>>4)&0x0F);
    				}

    len = write(fd,reBuf,len);
    
                     return len;



}
bool SerialCommunication::processMessage(Message* msg){
       
	return true;
}
void SerialCommunication::run(){

	char rcv_buf[21];
	int data_len=255;
//int d;
	//int err;
	//d = UART0_Open(d,"/dev/ttyS0");
     // do{
	//                 err = UART0_Init(d,115200,0,8,1,'N');
	//                 printf("Set Port Exactly!\n");
	    //  }while(-1== err || -1 == d);
     //tcflush(d,TCIOFLUSH);
      // int flags=fcntl(d,F_SETFL,0);
       //fcntl(d,F_SETFL,flags | O_NONBLOCK);
	while (1) {

			//handleMessage(UART0_Recv(fd, rcv_buf,data_len));
UART0_Recv(fd, rcv_buf,data_len);
 //printf("WWCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC");
		}
#ifdef MM
int fd,c, res;
 struct termios oldtio,newtio;
 char buf[255];
unsigned int temp = 0;
/* 
 开启数据机装置以读取并写入而不以控制 tty 的模式
 因为我们不想程序在送出 CTRL-C 后就被杀掉.
*/
 fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY ); 
 if (fd <0) {perror(MODEMDEVICE); exit(-1); }
 
 tcgetattr(fd,&oldtio); /* 储存目前的序列埠设定 */
 bzero(&newtio, sizeof(newtio)); /* 清除结构体以放入新的序列埠设定值 */
 
/* 
 BAUDRATE: 设定 bps 的速度. 你也可以用 cfsetispeed 及 cfsetospeed 来设定.
 CRTSCTS : 输出资料的硬件流量控制 (只能在具完整线路的缆线下工作
      参考 Serial-HOWTO 第七节)
 CS8   : 8n1 (8 位元, 不做同位元检查,1 个终止位元)
 CLOCAL : 本地连线, 不具数据机控制功能
 CREAD  : 致能接收字元
*/
 newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
  newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
/*
 IGNPAR : 忽略经同位元检查后, 错误的位元组
 ICRNL  : 比 CR 对应成 NL (否则当输入信号有 CR 时不会终止输入)
      在不然把装置设定成 raw 模式(没有其它的输入处理)
*/
 newtio.c_iflag = IGNPAR;
  newtio.c_iflag &=~(ICRNL | IGNCR);
newtio.c_oflag &=~(ONLCR | OCRNL);
newtio.c_iflag &=~(IXON | IXOFF | IXANY);
/*
 Raw 模式输出.
*/
 newtio.c_oflag = 0;
  
/*
 ICANON : 致能标准输入, 使所有回应机能停用, 并不送出信号以叫用程序
*/
 newtio.c_lflag = 0;
  
/* 
 初始化所有的控制特性
 预设值可以在 /usr/include/termios.h 找到, 在注解中也有,
 但我们在这不需要看它们
*/

 
 newtio.c_cc[VTIME]  = 0;   /* 不使用分割字元组的计时器 */
 newtio.c_cc[VMIN]   = 1;   /* 在读取到 1 个字元前先停止 */
/* 
 现在清除数据机线并启动序列埠的设定
*/
 tcflush(fd, TCIFLUSH);
 tcsetattr(fd,TCSANOW,&newtio);
 
/*
 终端机设定完成, 现在处理输入信号
 在这个范例, 在一行的开始处输入 'z' 会退出此程序.
*/
 while (1) {   /* 回圈会在我们发出终止的信号后跳出 */
 /* 即使输入超过 255 个字元, 读取的程序段还是会一直等到行终结符出现才停止.
  如果读到的字元组低于正确存在的字元组, 则所剩的字元会在下一次读取时取得.
  res 用来存放真正读到的字元组个数 */
  res = read(fd,buf,255); 

  //buf[res]=0;       /* 设定字串终止字元, 所以我们能用 printf */
  printf(":%s:%d/n", buf, res);
	buf[res] = '\0';

	//fflush(stdout);
	std::string string = std::string(buf);

	printf("Datagram is: ");

	for (int i = 0; i < res; ++i) {
		temp = buf[i];

		printf("%02x ", (unsigned char) temp);
	}
 }
 /* 回存旧的序列埠设定值 */
 tcsetattr(fd,TCSANOW,&oldtio);
#endif
}
void SerialCommunication::setFd(int fd){
	this->fd = fd;
}
}
