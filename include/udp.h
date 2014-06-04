#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define UDP_BUFFER_SIZE 1024
#define PORT 5000
#define bool int
#define ture 1
#define false 0

//接收的包类型
#define CAP 0
#define CARD 1

//发送的包类型
#define BAR 2
#define LED 3
#define CLIENT 4

static int sock_fd;
struct sockaddr_in address;
struct sockaddr_in broadcast;

struct msg
{
	int msg_type;
	int pos;
	int action;
};

struct msgCard
{
	int msg_type;
	int pos;
	char id_or_time[7];
	char owner_or_fee[9];
};

struct msg msg_recv,msg_send;
struct msgCard msgCard_recv,msgCard_send;

/*
 * 功能：创建套接字并绑定接收地址，设置广播地址
 * 参数: 无
 * 返回值：void
*/
void udp();

/*
 * 功能：发送消息
 * 参数: msg_type -消息类型 
 		pos		 -接收端的位置
 		action   -接收段需要执行的操作
 * 返回值：成功返回ture，失败返回false
*/
bool send_msg(int msg_type,int pos,int action);


/*
 * 功能：阻塞的接收消息
 * 参数: 无
 * 返回值：接受的包的类型，即msg_type
*/
int recv_msg();
