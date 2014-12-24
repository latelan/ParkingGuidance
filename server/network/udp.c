#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "udp.h"

//struct msg msg_recv,msg_send;
//struct msgCard msgCard_recv,msgCard_send;
/*
 * 功能：创建套接字并绑定接收地址，设置广播地址
 * 参数: 无
 * 返回值：void
*/
void udp()
{
	int ret;
	/*设置绑定地址*/
	memset(&address,0,sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(PORT);

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
	if(ret == -1)
	{
		printf("Error: set socket failed.\n");
		exit(0);
	}
	//绑定UDP地址
	ret = bind(sock_fd,(struct sockaddr *)&address,sizeof(address));
	if(ret == -1)
	{
		printf("Error: bind socket failed.\n");
		exit(0);
	}
}

/*
 * 功能：发送消息
 * 参数: msg_type -消息类型 
 		pos		 -接收端的位置
 		action   -接收段需要执行的操作
 * 返回值：成功返回ture，失败返回false
*/
bool send_msg(int msg_type,int pos,int action)
{
	int ret;
	int len = sizeof(broadcast);

	memset(&broadcast,0,sizeof(broadcast));
	broadcast.sin_family = AF_INET;
	broadcast.sin_port = htons(PORT);
	broadcast.sin_addr.s_addr = htonl(INADDR_BROADCAST);

//	printf("socket fd %d.\n",sock_fd);
	memset(&msg_send,-1,sizeof(msg_send));
//	printf("in send_msg: type %d  pos %d action %d\n",msg_type,pos,action);
	switch(msg_type)
	{
		case BAR:
			{
				msg_send.msg_type = msg_type;
				msg_send.pos = pos;
				msg_send.action = action;
			};break;
		case LED:
			{
//				printf("to LED\n");
				msg_send.msg_type = msg_type;
				msg_send.pos = pos;
				msg_send.action = action;
			};break;
		case CLIENT:
			{
				msg_send.msg_type = msg_type;
				msg_send.pos = pos;
				msg_send.action = action;
			};break;
		case CARD:
			{
				msgCard_send.msg_type = msg_type;
				msgCard_send.pos = pos;
				sendto(sock_fd,&msgCard_send,sizeof(msgCard_send),0,(struct sockaddr *)&broadcast,len);
			};break;
		case FEE:
			{
				msgCard_send.msg_type = msg_type;
				msgCard_send.pos = pos;
				sendto(sock_fd,&msgCard_send,sizeof(msgCard_send),0,(struct sockaddr *)&broadcast,len);
				break;
			}
		default:return false;
	}
	
	if (msg_type == BAR || msg_type == LED || msg_type == CLIENT)
	{
		ret = sendto(sock_fd,&msg_send,sizeof(msg_send),0,(struct sockaddr *)&broadcast,len); 
		memset(&broadcast,0,sizeof(broadcast));

		if (ret<=0)
		{
			 printf("msg send failed\n");
			 return false;
		}
	}
	return ture;
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
	memset(&msgCard_recv,-1,sizeof(msgCard_recv));

	socklen_t len = sizeof(broadcast);
	ret = recvfrom(sock_fd,buffer,1024-1,0,(struct sockaddr *)&broadcast,&len);
	if(ret <= 0)
	{
		printf("ret = %d\n",ret );
		return -1;
	}

	memcpy(&msg_recv,buffer,sizeof(msg_recv)); 
	

	if (msg_recv.msg_type == CAP)
	{
	//	printf("recv a CAP\n");
		return CAP;
	}
	else if (msg_recv.msg_type == CARD)
	{
	//	printf("recv a CARD\n");
		memcpy(&msgCard_recv,buffer,sizeof(msgCard_recv));
		return CARD;
	}
	return -1; 
}
