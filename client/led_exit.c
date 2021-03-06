#include <Arduino.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/select.h>
#include <errno.h>

//IO配置
#define LEDARRAY_D 2
#define LEDARRAY_C 3
#define LEDARRAY_B 4
#define LEDARRAY_A 5
#define LEDARRAY_G 6
#define LEDARRAY_DI 7
#define LEDARRAY_CLK 8
#define LEDARRAY_LAT 9

#define led 13


#define Num_Word 1				//液晶能显示的汉字个数
#define UDP_BUFFER_SIZE 1024
#define PORT 5000
#define bool int
#define ture 1
//接收的包类型
#define CAP 0
#define CARD 1
//发送的包类型
#define BAR 2
#define LED 3
#define FEE 5
int pos = 14;
static int sock_fd;
struct sockaddr_in address;
struct sockaddr_in broadcast;

struct msg
{
	int msg_type;
	int pos;
	int action;
}msg_recv;
struct msgCard
{
	int msg_type;
	int pos;
	char id_or_time[7];
	char owner_or_fee[9];
}msgCard_recv;

void set_nonblocking();
void Clear_Display();
void Send( unsigned char dat);
void Calc_Shift();
void Display(unsigned char dat[][32]);					
void Scan_Line( unsigned char m);


/*
 * 功能：创建套接字并绑定接收地址，设置广播地址
 * 参数: 无
 * 返回值：void
 */
void udp()
{
	int ret;

	memset(&address,0,sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	address.sin_addr.s_addr = htonl(INADDR_ANY);

	//设置接收/发送广播地址
	memset(&broadcast,0,sizeof(broadcast));
	broadcast.sin_family = AF_INET;
	broadcast.sin_port = htons(PORT);
	broadcast.sin_addr.s_addr = htonl(INADDR_BROADCAST);

	/*创建UDP socket，设置该套接字为广播类型*/
	sock_fd = socket(AF_INET,SOCK_DGRAM,0);
	assert(sock_fd >= 0);
	const int opt = 1;
	ret = setsockopt(sock_fd,SOL_SOCKET,SO_BROADCAST,(char *)&opt,sizeof(opt));
	assert(ret != -1);
	//绑定UDP地址
	ret = bind(sock_fd,(struct sockaddr *)&address,sizeof(address));
	assert(ret != -1);
	set_nonblocking();
}


/*
 * 功能：阻塞的接收消息
 * 参数: 无
 * 返回值：接受的包的类型，即msg_type
 */
int recv_msg()
{
	int ret;
	char buffer[1024];

	memset(buffer,'\0',UDP_BUFFER_SIZE);
	memset(&msg_recv,-1,sizeof(msg_recv));

	socklen_t len = sizeof(broadcast);
	ret = recvfrom(sock_fd,buffer,1024-1,0,(struct sockaddr *)&broadcast,&len);
	if(ret <= 0)
	{
		return -1;
	}

	memcpy(&msg_recv,buffer,sizeof(msg_recv)); 
	if(msg_recv.pos == pos) 
	{
		if (msg_recv.msg_type == BAR)
		{
			return BAR;
		}
		if (msg_recv.msg_type == FEE)
		{
			memcpy(&msgCard_recv,buffer,sizeof(msgCard_recv));
			return FEE;
		}
	} 
}

void set_nonblocking()
{
	int old_opt = fcntl(sock_fd,F_GETFL);
	int new_opt = old_opt | O_NONBLOCK;
	fcntl(sock_fd,F_SETFL,new_opt);
}


unsigned char Display_Buffer[2];
unsigned char Display_Swap_Buffer[Num_Word][32]={0};					//显示缓冲区
bool Shift_Bit = 0;
bool Flag_Shift = 0;
unsigned char Timer0_Count = 0;
unsigned char temp = 0x80;
unsigned char Shift_Count = 0;
unsigned char Display_Word_Count = 0;

#define Num_Of_Word 11 
const unsigned char Word1[Num_Of_Word][32] = 
{
	0xFF,0xFF,0x83,0xBB,0xB8,0xBB,0x83,0xBA,0xBB,0xBB,0x83,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xEF,0xEF,0xEF,0xEF,0x01,0xEF,0xEF,0xEF,0x6F,0x6F,0xEF,0xEF,0xEF,0xEF,0xAF,0xDF,/*"时"*/

	0xF7,0xF7,0xF7,0xF7,0xF6,0xF1,0xF7,0x00,0xF5,0xF6,0xF7,0xF7,0xF6,0xF5,0xF3,0xF7,
	0xFF,0xEF,0xDF,0xBF,0x7F,0xFF,0xFF,0x01,0xFF,0xFF,0x7F,0xBF,0xCF,0xE3,0xF7,0xFF,/*"长"*/

	0xFF,0xFF,0xFF,0xFC,0xF3,0xE7,0xCF,0xCF,0xCF,0xCF,0xCF,0xE7,0xF3,0xFC,0xFF,0xFF,
	0xFF,0xFF,0xFF,0x0F,0xE7,0xF3,0xF9,0xF9,0xF9,0xF9,0xF9,0xF3,0xE7,0x1F,0xFF,0xFF,/*"0"*/

	0xFF,0xFF,0xFF,0xFF,0xE0,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xE0,0xFF,0xFF,
	0xFF,0xFF,0xFF,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x07,0xFF,0xFF,/*"1"*/

	0xFE,0xFE,0xFE,0xFE,0xF6,0xF2,0xE6,0xEE,0xDE,0xBE,0x7E,0xFE,0xFE,0xFE,0xFA,0xFD,
	0xFF,0xFF,0xFF,0xFF,0xBF,0xDF,0xEF,0xE7,0xF3,0xF9,0xFB,0xFF,0xFF,0xFF,0xFF,0xFF,/*"小"*/

	0xFF,0xFF,0x83,0xBB,0xB8,0xBB,0x83,0xBA,0xBB,0xBB,0x83,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xEF,0xEF,0xEF,0xEF,0x01,0xEF,0xEF,0xEF,0x6F,0x6F,0xEF,0xEF,0xEF,0xEF,0xAF,0xDF,/*"时"*/

	0xFD,0xC0,0xFD,0xC0,0xDD,0xC0,0xFB,0xF7,0xE0,0x97,0xF6,0xF6,0xF6,0xFD,0xF3,0xCF,
	0xBF,0x07,0xB7,0x07,0xBF,0x01,0xBD,0xB5,0x0B,0xEF,0xEF,0xEF,0xEF,0x3F,0xC7,0xEF,/*"费"*/

	0xFF,0xE0,0xEF,0xEF,0xEF,0xE0,0xEF,0xEF,0xEF,0xE0,0xEF,0xEF,0xDF,0xDF,0xBF,0x7F,
	0xFF,0x03,0x7B,0x7B,0x7B,0x03,0x7B,0x7B,0x7B,0x03,0x7B,0x7B,0x7B,0x7B,0x6B,0x77,/*"用"*/

	0xFF,0xFF,0xFF,0xFC,0xF3,0xE7,0xCF,0xCF,0xCF,0xCF,0xCF,0xE7,0xF3,0xFC,0xFF,0xFF,
	0xFF,0xFF,0xFF,0x0F,0xE7,0xF3,0xF9,0xF9,0xF9,0xF9,0xF9,0xF3,0xE7,0x1F,0xFF,0xFF,/*"0"*/

	0xFF,0xFF,0xFF,0xF0,0xF7,0xF7,0xEF,0xE8,0xE7,0xFF,0xFF,0xCF,0xDF,0xE0,0xFF,0xFF,
	0xFF,0xFF,0xFF,0x03,0xFF,0xFF,0xFF,0x0F,0xE7,0xF3,0xF3,0xF3,0xC7,0x1F,0xFF,0xFF,/*"5"*/

	0xFF,0xE0,0xFF,0xFF,0xFF,0x80,0xFB,0xFB,0xFB,0xFB,0xF7,0xF7,0xEF,0xDF,0xBF,0xFF,
	0xFF,0x07,0xFF,0xFF,0xFF,0x01,0xBF,0xBF,0xBF,0xBF,0xBF,0xBF,0xBD,0xBD,0xC1,0xFF,/*"元"*/

};

const unsigned char Word2[Num_Of_Word][32] = 
{
	0xFF,0xFF,0x83,0xBB,0xB8,0xBB,0x83,0xBA,0xBB,0xBB,0x83,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xEF,0xEF,0xEF,0xEF,0x01,0xEF,0xEF,0xEF,0x6F,0x6F,0xEF,0xEF,0xEF,0xEF,0xAF,0xDF,/*"时"*/

	0xF7,0xF7,0xF7,0xF7,0xF6,0xF1,0xF7,0x00,0xF5,0xF6,0xF7,0xF7,0xF6,0xF5,0xF3,0xF7,
	0xFF,0xEF,0xDF,0xBF,0x7F,0xFF,0xFF,0x01,0xFF,0xFF,0x7F,0xBF,0xCF,0xE3,0xF7,0xFF,/*"长"*/

	0xFF,0xFF,0xFF,0xFC,0xF3,0xE7,0xCF,0xCF,0xCF,0xCF,0xCF,0xE7,0xF3,0xFC,0xFF,0xFF,
	0xFF,0xFF,0xFF,0x0F,0xE7,0xF3,0xF9,0xF9,0xF9,0xF9,0xF9,0xF3,0xE7,0x1F,0xFF,0xFF,/*"0"*/

	0xFF,0xFF,0xFF,0xF8,0xE7,0xDF,0xCF,0xFF,0xFF,0xFF,0xFC,0xF3,0xEF,0xC0,0xFF,0xFF,
	0xFF,0xFF,0xFF,0x0F,0xE3,0xF3,0xF3,0xE7,0xDF,0x3F,0xFF,0xFB,0xF3,0x07,0xFF,0xFF,/*"2"*/

	0xFE,0xFE,0xFE,0xFE,0xF6,0xF2,0xE6,0xEE,0xDE,0xBE,0x7E,0xFE,0xFE,0xFE,0xFA,0xFD,
	0xFF,0xFF,0xFF,0xFF,0xBF,0xDF,0xEF,0xE7,0xF3,0xF9,0xFB,0xFF,0xFF,0xFF,0xFF,0xFF,/*"小"*/

	0xFF,0xFF,0x83,0xBB,0xB8,0xBB,0x83,0xBA,0xBB,0xBB,0x83,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xEF,0xEF,0xEF,0xEF,0x01,0xEF,0xEF,0xEF,0x6F,0x6F,0xEF,0xEF,0xEF,0xEF,0xAF,0xDF,/*"时"*/

	0xFD,0xC0,0xFD,0xC0,0xDD,0xC0,0xFB,0xF7,0xE0,0x97,0xF6,0xF6,0xF6,0xFD,0xF3,0xCF,
	0xBF,0x07,0xB7,0x07,0xBF,0x01,0xBD,0xB5,0x0B,0xEF,0xEF,0xEF,0xEF,0x3F,0xC7,0xEF,/*"费"*/

	0xFF,0xE0,0xEF,0xEF,0xEF,0xE0,0xEF,0xEF,0xEF,0xE0,0xEF,0xEF,0xDF,0xDF,0xBF,0x7F,
	0xFF,0x03,0x7B,0x7B,0x7B,0x03,0x7B,0x7B,0x7B,0x03,0x7B,0x7B,0x7B,0x7B,0x6B,0x77,/*"用"*/

	0xFF,0xFF,0xFF,0xFF,0xE0,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xE0,0xFF,0xFF,
	0xFF,0xFF,0xFF,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x07,0xFF,0xFF,/*"1"*/

	0xFF,0xFF,0xFF,0xFC,0xF3,0xE7,0xCF,0xCF,0xCF,0xCF,0xCF,0xE7,0xF3,0xFC,0xFF,0xFF,
	0xFF,0xFF,0xFF,0x0F,0xE7,0xF3,0xF9,0xF9,0xF9,0xF9,0xF9,0xF3,0xE7,0x1F,0xFF,0xFF,/*"0"*/

	0xFF,0xE0,0xFF,0xFF,0xFF,0x80,0xFB,0xFB,0xFB,0xFB,0xF7,0xF7,0xEF,0xDF,0xBF,0xFF,
	0xFF,0x07,0xFF,0xFF,0xFF,0x01,0xBF,0xBF,0xBF,0xBF,0xBF,0xBF,0xBD,0xBD,0xC1,0xFF,/*"元"*/

};

const unsigned char Word3[Num_Of_Word][32] = 
{
	0xFF,0xFF,0x83,0xBB,0xB8,0xBB,0x83,0xBA,0xBB,0xBB,0x83,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xEF,0xEF,0xEF,0xEF,0x01,0xEF,0xEF,0xEF,0x6F,0x6F,0xEF,0xEF,0xEF,0xEF,0xAF,0xDF,/*"时"*/

	0xF7,0xF7,0xF7,0xF7,0xF6,0xF1,0xF7,0x00,0xF5,0xF6,0xF7,0xF7,0xF6,0xF5,0xF3,0xF7,
	0xFF,0xEF,0xDF,0xBF,0x7F,0xFF,0xFF,0x01,0xFF,0xFF,0x7F,0xBF,0xCF,0xE3,0xF7,0xFF,/*"长"*/

	0xFF,0xFF,0xFF,0xFC,0xF3,0xE7,0xCF,0xCF,0xCF,0xCF,0xCF,0xE7,0xF3,0xFC,0xFF,0xFF,
	0xFF,0xFF,0xFF,0x0F,0xE7,0xF3,0xF9,0xF9,0xF9,0xF9,0xF9,0xF3,0xE7,0x1F,0xFF,0xFF,/*"0"*/

	0xFF,0xFF,0xFF,0xF0,0xCF,0xCF,0xFF,0xFC,0xFF,0xFF,0xFF,0xCF,0xCF,0xF0,0xFF,0xFF,
	0xFF,0xFF,0xFF,0x1F,0xC7,0xE7,0xCF,0x3F,0xC7,0xF3,0xF3,0xF3,0xC7,0x1F,0xFF,0xFF, /*"3"*/

	0xFE,0xFE,0xFE,0xFE,0xF6,0xF2,0xE6,0xEE,0xDE,0xBE,0x7E,0xFE,0xFE,0xFE,0xFA,0xFD,
	0xFF,0xFF,0xFF,0xFF,0xBF,0xDF,0xEF,0xE7,0xF3,0xF9,0xFB,0xFF,0xFF,0xFF,0xFF,0xFF,/*"小"*/

	0xFF,0xFF,0x83,0xBB,0xB8,0xBB,0x83,0xBA,0xBB,0xBB,0x83,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xEF,0xEF,0xEF,0xEF,0x01,0xEF,0xEF,0xEF,0x6F,0x6F,0xEF,0xEF,0xEF,0xEF,0xAF,0xDF,/*"时"*/

	0xFD,0xC0,0xFD,0xC0,0xDD,0xC0,0xFB,0xF7,0xE0,0x97,0xF6,0xF6,0xF6,0xFD,0xF3,0xCF,
	0xBF,0x07,0xB7,0x07,0xBF,0x01,0xBD,0xB5,0x0B,0xEF,0xEF,0xEF,0xEF,0x3F,0xC7,0xEF,/*"费"*/

	0xFF,0xE0,0xEF,0xEF,0xEF,0xE0,0xEF,0xEF,0xEF,0xE0,0xEF,0xEF,0xDF,0xDF,0xBF,0x7F,
	0xFF,0x03,0x7B,0x7B,0x7B,0x03,0x7B,0x7B,0x7B,0x03,0x7B,0x7B,0x7B,0x7B,0x6B,0x77,/*"用"*/

	0xFF,0xFF,0xFF,0xFF,0xE0,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xE0,0xFF,0xFF,
	0xFF,0xFF,0xFF,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x07,0xFF,0xFF,/*"1"*/

	0xFF,0xFF,0xFF,0xF0,0xF7,0xF7,0xEF,0xE8,0xE7,0xFF,0xFF,0xCF,0xDF,0xE0,0xFF,0xFF,
	0xFF,0xFF,0xFF,0x03,0xFF,0xFF,0xFF,0x0F,0xE7,0xF3,0xF3,0xF3,0xC7,0x1F,0xFF,0xFF,/*"5"*/

	0xFF,0xE0,0xFF,0xFF,0xFF,0x80,0xFB,0xFB,0xFB,0xFB,0xF7,0xF7,0xEF,0xDF,0xBF,0xFF,
	0xFF,0x07,0xFF,0xFF,0xFF,0x01,0xBF,0xBF,0xBF,0xBF,0xBF,0xBF,0xBD,0xBD,0xC1,0xFF,/*"元"*/

};

const unsigned char Word4[Num_Of_Word][32] = 
{
	0xFF,0xFF,0x83,0xBB,0xB8,0xBB,0x83,0xBA,0xBB,0xBB,0x83,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xEF,0xEF,0xEF,0xEF,0x01,0xEF,0xEF,0xEF,0x6F,0x6F,0xEF,0xEF,0xEF,0xEF,0xAF,0xDF,/*"时"*/

	0xF7,0xF7,0xF7,0xF7,0xF6,0xF1,0xF7,0x00,0xF5,0xF6,0xF7,0xF7,0xF6,0xF5,0xF3,0xF7,
	0xFF,0xEF,0xDF,0xBF,0x7F,0xFF,0xFF,0x01,0xFF,0xFF,0x7F,0xBF,0xCF,0xE3,0xF7,0xFF,/*"长"*/

	0xFF,0xFF,0xFF,0xFC,0xF3,0xE7,0xCF,0xCF,0xCF,0xCF,0xCF,0xE7,0xF3,0xFC,0xFF,0xFF,
	0xFF,0xFF,0xFF,0x0F,0xE7,0xF3,0xF9,0xF9,0xF9,0xF9,0xF9,0xF3,0xE7,0x1F,0xFF,0xFF,/*"0"*/

	0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFD,0xFB,0xE7,0xDF,0x80,0xFF,0xFF,0xFC,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xCF,0x0F,0x8F,0x8F,0x8F,0x8F,0x8F,0x01,0x8F,0x8F,0x01,0xFF,0xFF,/*"4"*/

	0xFE,0xFE,0xFE,0xFE,0xF6,0xF2,0xE6,0xEE,0xDE,0xBE,0x7E,0xFE,0xFE,0xFE,0xFA,0xFD,
	0xFF,0xFF,0xFF,0xFF,0xBF,0xDF,0xEF,0xE7,0xF3,0xF9,0xFB,0xFF,0xFF,0xFF,0xFF,0xFF,/*"小"*/

	0xFF,0xFF,0x83,0xBB,0xB8,0xBB,0x83,0xBA,0xBB,0xBB,0x83,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xEF,0xEF,0xEF,0xEF,0x01,0xEF,0xEF,0xEF,0x6F,0x6F,0xEF,0xEF,0xEF,0xEF,0xAF,0xDF,/*"时"*/

	0xFD,0xC0,0xFD,0xC0,0xDD,0xC0,0xFB,0xF7,0xE0,0x97,0xF6,0xF6,0xF6,0xFD,0xF3,0xCF,
	0xBF,0x07,0xB7,0x07,0xBF,0x01,0xBD,0xB5,0x0B,0xEF,0xEF,0xEF,0xEF,0x3F,0xC7,0xEF,/*"费"*/

	0xFF,0xE0,0xEF,0xEF,0xEF,0xE0,0xEF,0xEF,0xEF,0xE0,0xEF,0xEF,0xDF,0xDF,0xBF,0x7F,
	0xFF,0x03,0x7B,0x7B,0x7B,0x03,0x7B,0x7B,0x7B,0x03,0x7B,0x7B,0x7B,0x7B,0x6B,0x77,/*"用"*/

	0xFF,0xFF,0xFF,0xF8,0xE7,0xDF,0xCF,0xFF,0xFF,0xFF,0xFC,0xF3,0xEF,0xC0,0xFF,0xFF,
	0xFF,0xFF,0xFF,0x0F,0xE3,0xF3,0xF3,0xE7,0xDF,0x3F,0xFF,0xFB,0xF3,0x07,0xFF,0xFF,/*"2"*/

	0xFF,0xFF,0xFF,0xFC,0xF3,0xE7,0xCF,0xCF,0xCF,0xCF,0xCF,0xE7,0xF3,0xFC,0xFF,0xFF,
	0xFF,0xFF,0xFF,0x0F,0xE7,0xF3,0xF9,0xF9,0xF9,0xF9,0xF9,0xF3,0xE7,0x1F,0xFF,0xFF,/*"0"*/

	0xFF,0xE0,0xFF,0xFF,0xFF,0x80,0xFB,0xFB,0xFB,0xFB,0xF7,0xF7,0xEF,0xDF,0xBF,0xFF,
	0xFF,0x07,0xFF,0xFF,0xFF,0x01,0xBF,0xBF,0xBF,0xBF,0xBF,0xBF,0xBD,0xBD,0xC1,0xFF,/*"元"*/

};

const unsigned char Word5[Num_Of_Word][32] = 
{
	0xFF,0xFF,0x83,0xBB,0xB8,0xBB,0x83,0xBA,0xBB,0xBB,0x83,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xEF,0xEF,0xEF,0xEF,0x01,0xEF,0xEF,0xEF,0x6F,0x6F,0xEF,0xEF,0xEF,0xEF,0xAF,0xDF,/*"时"*/

	0xF7,0xF7,0xF7,0xF7,0xF6,0xF1,0xF7,0x00,0xF5,0xF6,0xF7,0xF7,0xF6,0xF5,0xF3,0xF7,
	0xFF,0xEF,0xDF,0xBF,0x7F,0xFF,0xFF,0x01,0xFF,0xFF,0x7F,0xBF,0xCF,0xE3,0xF7,0xFF,/*"长"*/

	0xFF,0xFF,0xFF,0xFC,0xF3,0xE7,0xCF,0xCF,0xCF,0xCF,0xCF,0xE7,0xF3,0xFC,0xFF,0xFF,
	0xFF,0xFF,0xFF,0x0F,0xE7,0xF3,0xF9,0xF9,0xF9,0xF9,0xF9,0xF3,0xE7,0x1F,0xFF,0xFF,/*"0"*/

	0xFF,0xFF,0xFF,0xF0,0xF7,0xF7,0xEF,0xE8,0xE7,0xFF,0xFF,0xCF,0xDF,0xE0,0xFF,0xFF,
	0xFF,0xFF,0xFF,0x03,0xFF,0xFF,0xFF,0x0F,0xE7,0xF3,0xF3,0xF3,0xC7,0x1F,0xFF,0xFF,/*"5"*/

	0xFE,0xFE,0xFE,0xFE,0xF6,0xF2,0xE6,0xEE,0xDE,0xBE,0x7E,0xFE,0xFE,0xFE,0xFA,0xFD,
	0xFF,0xFF,0xFF,0xFF,0xBF,0xDF,0xEF,0xE7,0xF3,0xF9,0xFB,0xFF,0xFF,0xFF,0xFF,0xFF,/*"小"*/

	0xFF,0xFF,0x83,0xBB,0xB8,0xBB,0x83,0xBA,0xBB,0xBB,0x83,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xEF,0xEF,0xEF,0xEF,0x01,0xEF,0xEF,0xEF,0x6F,0x6F,0xEF,0xEF,0xEF,0xEF,0xAF,0xDF,/*"时"*/

	0xFD,0xC0,0xFD,0xC0,0xDD,0xC0,0xFB,0xF7,0xE0,0x97,0xF6,0xF6,0xF6,0xFD,0xF3,0xCF,
	0xBF,0x07,0xB7,0x07,0xBF,0x01,0xBD,0xB5,0x0B,0xEF,0xEF,0xEF,0xEF,0x3F,0xC7,0xEF,/*"费"*/

	0xFF,0xE0,0xEF,0xEF,0xEF,0xE0,0xEF,0xEF,0xEF,0xE0,0xEF,0xEF,0xDF,0xDF,0xBF,0x7F,
	0xFF,0x03,0x7B,0x7B,0x7B,0x03,0x7B,0x7B,0x7B,0x03,0x7B,0x7B,0x7B,0x7B,0x6B,0x77,/*"用"*/

	0xFF,0xFF,0xFF,0xF8,0xE7,0xDF,0xCF,0xFF,0xFF,0xFF,0xFC,0xF3,0xEF,0xC0,0xFF,0xFF,
	0xFF,0xFF,0xFF,0x0F,0xE3,0xF3,0xF3,0xE7,0xDF,0x3F,0xFF,0xFB,0xF3,0x07,0xFF,0xFF,/*"2"*/

	0xFF,0xFF,0xFF,0xF0,0xF7,0xF7,0xEF,0xE8,0xE7,0xFF,0xFF,0xCF,0xDF,0xE0,0xFF,0xFF,
	0xFF,0xFF,0xFF,0x03,0xFF,0xFF,0xFF,0x0F,0xE7,0xF3,0xF3,0xF3,0xC7,0x1F,0xFF,0xFF,/*"5"*/

	0xFF,0xE0,0xFF,0xFF,0xFF,0x80,0xFB,0xFB,0xFB,0xFB,0xF7,0xF7,0xEF,0xDF,0xBF,0xFF,
	0xFF,0x07,0xFF,0xFF,0xFF,0x01,0xBF,0xBF,0xBF,0xBF,0xBF,0xBF,0xBD,0xBD,0xC1,0xFF,/*"元"*/

};

const unsigned char Word13[Num_Of_Word][32] = 
{
	0xFF,0xB8,0xCF,0xDC,0xFF,0xF8,0x0F,0xEC,0xED,0xEC,0xED,0xEC,0xE9,0xE5,0xED,0xFD,
	0xBF,0x03,0xBF,0x07,0xBF,0x01,0xFF,0x07,0xF7,0x07,0xF7,0x07,0xF7,0xF7,0xD7,0xEF,

	0xC0,0xDF,0xDF,0xDF,0xC0,0xDD,0xDD,0xA0,0xAD,0xAD,0xAD,0xAD,0xAD,0x6C,0xFD,0xFD,
	0x7D,0x7D,0x6D,0x6D,0x6D,0xED,0xED,0x2D,0xAD,0xAD,0xAD,0xAD,0xBD,0x3D,0x75,0xFB,

	0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0x80,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,
	0xFF,0xFF,0x03,0xFF,0xFF,0xFF,0x01,0xFF,0xFF,0xBF,0xCF,0xE7,0xF3,0xF7,0xFF,0xFF,

	0xFE,0xFE,0xDE,0xDE,0xDE,0xDE,0xC0,0xDE,0xFE,0xDE,0xDE,0xDE,0xDE,0xC0,0xDF,0xFF,
	0xFF,0xFF,0xFB,0xFB,0xFB,0xFB,0x03,0xFB,0xFF,0xFB,0xFB,0xFB,0xFB,0x03,0xFB,0xFF,

	0xFE,0xFF,0xC0,0xDE,0xDE,0xC0,0xDD,0xDB,0xD0,0xDB,0xDF,0xC0,0xDF,0xBF,0xBF,0x7F,
	0xFF,0x7F,0x01,0xFF,0xFF,0x03,0xBF,0xBF,0x03,0xBF,0xBF,0x01,0xBF,0xBF,0xBF,0xBF,

	0xFF,0xFF,0xFF,0xFF,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,0xFF,0xEF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,

	0xFF,0xB8,0xCF,0xDC,0xFF,0xF8,0x0F,0xEC,0xED,0xEC,0xED,0xEC,0xE9,0xE5,0xED,0xFD,
	0xBF,0x03,0xBF,0x07,0xBF,0x01,0xFF,0x07,0xF7,0x07,0xF7,0x07,0xF7,0xF7,0xD7,0xEF,

	0xC0,0xDF,0xDF,0xDF,0xC0,0xDD,0xDD,0xA0,0xAD,0xAD,0xAD,0xAD,0xAD,0x6C,0xFD,0xFD,
	0x7D,0x7D,0x6D,0x6D,0x6D,0xED,0xED,0x2D,0xAD,0xAD,0xAD,0xAD,0xBD,0x3D,0x75,0xFB,

	0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0x80,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,
	0xFF,0xFF,0x03,0xFF,0xFF,0xFF,0x01,0xFF,0xFF,0xBF,0xCF,0xE7,0xF3,0xF7,0xFF,0xFF,

	0xFE,0xFE,0xDE,0xDE,0xDE,0xDE,0xC0,0xDE,0xFE,0xDE,0xDE,0xDE,0xDE,0xC0,0xDF,0xFF,
	0xFF,0xFF,0xFB,0xFB,0xFB,0xFB,0x03,0xFB,0xFF,0xFB,0xFB,0xFB,0xFB,0x03,0xFB,0xFF,

	0xFE,0xFF,0xC0,0xDE,0xDE,0xC0,0xDD,0xDB,0xD0,0xDB,0xDF,0xC0,0xDF,0xBF,0xBF,0x7F,
	0xFF,0x7F,0x01,0xFF,0xFF,0x03,0xBF,0xBF,0x03,0xBF,0xBF,0x01,0xBF,0xBF,0xBF,0xBF,
};



unsigned char Word[Num_Of_Word][32] = {0};
int time = -1;
int count = 0;
void setup()
{
	if(argc!=2)
	{                                                              
		printf("usage:led_exit pos<exit:14,15>");
		exit(0);
	}
	pos = atoi(argv[1]);
	pinMode(LEDARRAY_D, OUTPUT); 
	pinMode(LEDARRAY_C, OUTPUT);
	pinMode(LEDARRAY_B, OUTPUT);
	pinMode(LEDARRAY_A, OUTPUT);
	pinMode(LEDARRAY_G, OUTPUT);
	pinMode(LEDARRAY_DI, OUTPUT);
	pinMode(LEDARRAY_CLK, OUTPUT);
	pinMode(LEDARRAY_LAT, OUTPUT);
	udp();
	Clear_Display();
}

void loop()
{
	unsigned int i,m,n;
	int ret;
	pid_t pid;    
	fd_set read_set;
	struct timeval timeout;

	timeout.tv_sec = 0;
	timeout.tv_usec = 50;
	FD_ZERO(&read_set);
	FD_SET(sock_fd,&read_set);

	select(sock_fd+1,&read_set,NULL,NULL,&timeout);
	if(FD_ISSET(sock_fd,&read_set))
	{
		ret = recv_msg();
		if(ret == BAR)
		{
			pid = fork();
			if(pid == 0)
			{
		    	execl("/home/ubuntu/c_enviroment/output/test/motor","motor","3000",NULL);
				exit(0);
			}
		}
		if(ret == FEE)
		{
			time = atoi(msgCard_recv.id_or_time);
			Clear_Display();
			Display_Buffer[2];
			Display_Swap_Buffer[Num_Word][32]={0};         
			Shift_Bit = 0;
			Flag_Shift = 0;
			Timer0_Count = 0;
			temp = 0x80;
			Display_Word_Count = 0;
			Shift_Count = 0;

		}
	}

	count++;
	if(count > 800)
	{
		time = -1;
		count = 0;
		Clear_Display();
		Display_Buffer[2];
		Display_Swap_Buffer[Num_Word][32]={0};         
		Shift_Bit = 0;
		Flag_Shift = 0;
		Timer0_Count = 0;
		temp = 0x80;
		Display_Word_Count = 0;
		Shift_Count = 0;
	}

	switch(time)
	{
		case 1:
			for(m=0;m<Num_Of_Word;m++)
			{	for(n=0;n<32;n++)
				Word[m][n]=Word1[m][n];
			}break;	
		case 2:for(m=0;m<Num_Of_Word;m++)
			   {	for(n=0;n<32;n++)
				   Word[m][n]=Word2[m][n];
			   }break;
		case 3:for(m=0;m<Num_Of_Word;m++)
			   {	for(n=0;n<32;n++)
				   Word[m][n]=Word3[m][n];
			   }break;
		case 4:
			   for(m=0;m<Num_Of_Word;m++)
			   {	for(n=0;n<32;n++)
				   Word[m][n]=Word4[m][n];
			   }break;
		case 5:
			   for(m=0;m<Num_Of_Word;m++)
			   {	for(n=0;n<32;n++)
				   Word[m][n]=Word5[m][n];
			   }break;
		default:
			   {
				   for(m=0;m<Num_Of_Word;m++)
				   {	for(n=0;n<32;n++)
					   Word[m][n]=Word13[m][n];
				   }
				   break;
			   }
	}


	for(i = 0 ; i < 3; i ++)			//移动速度设定
	{
		Display(Display_Swap_Buffer);
	}
	Display_Word_Count = Shift_Count/16;				//计算当前显示第几个字
	Calc_Shift();
	Shift_Count++;
	if(Shift_Count == (Num_Of_Word+1)*16 )				//移动次数
	{
		Shift_Count = 0;				
	}	

}

//************************************************************
//清空缓冲区
//************************************************************
void Clear_Display()
{
	unsigned char i,j;
	for(j = 0 ; j < Num_Word; j++)
	{
		for(i = 0 ; i < 32 ;i++)
		{
			Display_Swap_Buffer[j][i] = 0xff;				//0=显示  1=不显示 	
		}
	}
}

//************************************************************
//计算移动数据,存在在缓冲器
//************************************************************
void Calc_Shift()
{
	unsigned char i;

	for(i = 0;i < 16;i++)
	{
		if((Display_Swap_Buffer[0][16+i]&0x80) == 0)									//前8位	移位
		{
			Display_Swap_Buffer[0][i] = (Display_Swap_Buffer[0][i] << 1)&0xfe; 			//最低位清零	
		}
		else
		{
			Display_Swap_Buffer[0][i] = (Display_Swap_Buffer[0][i] << 1)|0x01;			//最低位置一 	
		}

		if(Shift_Count%16 < 8 && Display_Word_Count < Num_Of_Word)
		{
			Shift_Bit = Word[Display_Word_Count][i]&temp;
		}
		else if(Shift_Count%16 < 16 && Display_Word_Count < Num_Of_Word)
		{
			Shift_Bit = Word[Display_Word_Count][16+i]&temp;
		}
		else
		{
			Shift_Bit = 1;						//把字移出缓冲区
		}

		if( Shift_Bit == 0)														//后8位移位
		{
			Display_Swap_Buffer[0][16+i] = (Display_Swap_Buffer[0][16+i] << 1)&0xfe; 		//最低位清零
		}
		else
		{
			Shift_Bit =	1;
			Display_Swap_Buffer[0][16+i] = (Display_Swap_Buffer[0][16+i] << 1)|0x01;		//最低位置一 			
		}

	}
	temp = (temp>>1)&0x7f;
	if(temp == 0x00)
	{
		temp = 0x80;
	}
}
//************************************************************
//num为字数  dat[][32]为字模的名称
//*************************************************************
void Display(unsigned char dat[][32])					
{
	unsigned char i;

	for( i = 0 ; i < 16 ; i++ )
	{
		digitalWrite(LEDARRAY_G, HIGH);		//更新数据时候关闭显示。等更新完数据,打开138显示行。防止重影。

		Display_Buffer[0] = dat[0][i];		
		Display_Buffer[1] = dat[0][i+16];

		Send(Display_Buffer[1]);
		Send(Display_Buffer[0]);

		digitalWrite(LEDARRAY_LAT, HIGH);					//锁存数据
		delayMicroseconds(1);

		digitalWrite(LEDARRAY_LAT, LOW);
		delayMicroseconds(1);

		Scan_Line(i);						//选择第i行

		digitalWrite(LEDARRAY_G, LOW);

		delayMicroseconds(300);;		//延时一段时间,让LED亮起来。				
	}	
}

//****************************************************
//扫描某一行
//****************************************************
void Scan_Line( unsigned char m)
{	
	switch(m)
	{
		case 0:			
			digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW); 					
			break;
		case 1:					
			digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH); 		
			break;
		case 2:					
			digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW); 		
			break;
		case 3:					
			digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH); 		
			break;
		case 4:
			digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW); 		
			break;
		case 5:
			digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH); 		
			break;
		case 6:
			digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW); 		
			break;
		case 7:
			digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH); 		
			break;
		case 8:
			digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW); 		
			break;
		case 9:
			digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH); 		
			break;	
		case 10:
			digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW); 		
			break;
		case 11:
			digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH); 		
			break;
		case 12:
			digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW); 		
			break;
		case 13:
			digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH); 		
			break;
		case 14:
			digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW); 		
			break;
		case 15:
			digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH); 		
			break;
		default : break;	
	}
}

//****************************************************
//发送数据
//****************************************************
void Send( unsigned char dat)
{
	unsigned char i;
	digitalWrite(LEDARRAY_CLK, LOW);
	//delayMicroseconds(1);;	
	digitalWrite(LEDARRAY_LAT, LOW);
	//delayMicroseconds(1);;

	for( i = 0 ; i < 8 ; i++ )
	{
		if( dat&0x01 )
		{
			digitalWrite(LEDARRAY_DI, HIGH);	
		}
		else
		{
			digitalWrite(LEDARRAY_DI, LOW);
		}


		digitalWrite(LEDARRAY_CLK, HIGH);				//上升沿发送数据
		//delayMicroseconds(1);;
		digitalWrite(LEDARRAY_CLK, LOW);
		//delayMicroseconds(1);;		
		dat >>= 1;

	}			
}
