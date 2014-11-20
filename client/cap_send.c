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

int sock_fd;
struct sockaddr_in address;
struct sockaddr_in broadcast;

struct msg
{
	int msg_type;
	int pos;
	int action;
}msg_recv,msg_send;

struct msgCard
{
	int msg_type;
	int pos;
	char id[7];
	char usr[9];
}msgCard_recv,msgCard_send;

/*创建UDP套接字，设置广播属性，并向子网内所有主机广播*/
void udp()
{
	int ret;
	/*设置绑定地址*/
/*	memset(&address,0,sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(PORT);*/

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
//	ret = bind(sock_fd,(struct sockaddr *)&address,sizeof(address));
//	assert(ret != -1);
}

bool send_msg(int msg_type,int pos,int action)
{
	int len;
	int ret;
	memset(&msg_send,-1,sizeof(msg_send));
	switch(msg_type)
	{
		case CAP://bar
			{
				msg_send.msg_type = msg_type;
				msg_send.pos = pos;
				msg_send.action = action;
				printf("send a CAP %d\n",(int)sizeof(msg_send));
			};break;
		case CARD://LED
			{
				msg_send.msg_type = msg_type;
				msg_send.pos = pos;
				msg_send.action = action;
				printf("send a CARD %d\n",(int)sizeof(msg_send));
			};break;
		case CLIENT:
			{
				msg_send.msg_type = msg_type;
				msg_send.pos = pos;
				msg_send.action = action;
				printf("send a CLIENT\n");
			};break;
		defalut:printf("UNKNOW sendmsg\n"); return false;
	}
	len = sizeof(broadcast);
	ret = sendto(sock_fd,&msg_send,sizeof(msg_send),0,(struct sockaddr *)&broadcast,len); 
	if (ret<=0)
	{
		 printf("msg send failed\n");
		 return false;
	}
	return ture;
}

int main(int argc, char const *argv[])
{
	if(argc != 4)
	{
		printf("Usage: %s enterpos[0,1] cardid ownername.\n",argv[0]);
		return 0;
	}
	udp();
	char * buf = "hello";
	int i = 10;
	int type;
	socklen_t len = sizeof(broadcast);
		msgCard_send.msg_type = CARD;
		msgCard_send.pos = atoi(argv[1]);
		strcpy(msgCard_send.id,argv[2]);
		strcpy(msgCard_send.usr,argv[3]);
		int ret = sendto(sock_fd,&msgCard_send,sizeof(msgCard_send),0,(struct sockaddr *)&broadcast,len); 
		if (ret <= 0)
		{
			printf("send failed\n" );
		}
	//	printf("%d  %d  %d\n",msgCard_send.msg_type, msg_send.pos,msg_send.action);
	return 0;
}
