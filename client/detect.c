#include <core.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>

#define PORT 5000
//接收的包类型
#define CAP 0

static int pos = 0;
static int sock_fd;
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
	char id_or_time[7];
	char owner_or_fee[9];
}msgCard_recv,msgCard_send;

/*
 * 功能：创建套接字并绑定接收地址，设置广播地址
 * 参数: 无
 * 返回值：void
*/
void udp()
{
	int ret;

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
}

int send_msg(int msg_type,int pos,int action)
{
	int ret;
	int len = sizeof(broadcast);
	memset(&msg_send,-1,sizeof(msg_send));
	
	msg_send.msg_type = msg_type;
	msg_send.pos = pos;
	msg_send.action = action;
	

	ret = sendto(sock_fd,&msg_send,sizeof(msg_send),0,(struct sockaddr *)&broadcast,len); 
	if (ret<=0)
	{
		 printf("msg send failed\n");
		 return false;
	}
	return ret;
}
const int trig = 10;
const int echo = 11;
int back_info = 0;

long microsecondsToInches(long microseconds)
{
	return (microseconds / 74 / 2);
}
long microsecondsToCentimeters(long microseconds)
{
	return (microseconds / 29 / 2);
}
void setup()
{
	if(argc!=2)
	{
		printf("usage: detect  pos");
		exit(0);
	}
	pos = atoi(argv[1]);
    udp();
	//初始化超声波传感器
	pinMode(echo,INPUT);
	pinMode(trig,OUTPUT);
	digitalWrite(trig,LOW);
	delay(20);
}
void loop()
{
	long duration,inches,cm;
	digitalWrite(trig,HIGH);
	delayMicroseconds(20);
	digitalWrite(trig,LOW);
	duration = pulseIn(echo,HIGH,1000000);
	inches = microsecondsToInches(duration);
	cm = microsecondsToCentimeters(duration);
        

	if( cm>=200 )//car out
	{	
       if(back_info == 1)
       {
		send_msg(CAP,pos,0);
		printf("%d cm\t  car out\n",cm);
        back_info = 0;
       }
	}
    else if(cm >= 10)
	{
        if(back_info == 0)
        {
			send_msg(CAP,pos,1);
			printf("%d cm\t car in\n",cm);	
			back_info = 1;
        }
	}
	delay(1000);
}
