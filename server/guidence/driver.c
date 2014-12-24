#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dijkstra.h"
#include "sql.h"
#include "data.h"
#include "udp.h"
#include "datetime.h"

#define FEE_PER_HOUR 5 // 每小时停车费用

/* 停车检测信息，赋值给发送包的action */
#define FULL 9				// 车库已满
#define UNVALID 10			// 用户信息不合法
#define EXIST 11			// 该卡正在使用中
#define LIFT_BAR 1 			// 抬杆
#define DONOT_LIFT_BAR 0	// 不抬杆

void logmaker(const char *log_records);
char log_path[100] = "./log/";


//struct msg msg_recv,msg_send;
//struct msgCard msgCard_recv,msgCard_send;
/**
 * 功能：检查当前是否还能停车
 * 参数：id_card - 车主所持卡ID
 * 		 emsg - 不能停车的原因
 * 返回值：可以停返回1，emsg指向NULL，不可以停返回0，emsg为详细
 */
int parking_check(const char *id_card)
{
	//int can_stop;
	char start_time[100];
	
	// 车库是否已满
	if(is_full() == 1)
	{
		printf("Msg: cann't park. parking lots is full.\n");
		logmaker("Msg: cann't park. parking lots is full.");
//		send_msg(BAR,msg_recv.pos,DONOT_LIFT_BAR);
		send_msg(LED,msg_recv.pos,FULL);
		return DONOT_LIFT_BAR;
	}
	
	// 是否是合法用户
	if(is_valid_user(id_card) == 0)
	{
		printf("Msg: cann't park. unvalid user.\n");
		logmaker("Msg: cann't park. unvalid user.");
//		send_msg(BAR,msg_recv.pos,DONOT_LIFT_BAR);
		send_msg(LED,msg_recv.pos,UNVALID);
		return DONOT_LIFT_BAR;
	}
	
	// 是否已停过车
	if(query_start_time(id_card,start_time) == 1)
	{	
		printf("Msg: cann't park. a previous parking records has found.\n");
		logmaker("Msg: cann't park. a previous parking records has found.");
//		send_msg(BAR,msg_recv.pos,DONOT_LIFT_BAR);
		send_msg(LED,msg_recv.pos,EXIST);
		return DONOT_LIFT_BAR;
	}
	
	//可以停车
	printf("Msg: can park.\n");
	send_msg(BAR,msg_recv.pos,LIFT_BAR);

	return LIFT_BAR;
}

/**
 * 功能：停车入库处理过程
 * 参数：enter_node - 入口节点
 * 		 id_card - 车主所持卡ID
 * 		 owner - 车主所持卡OWNER
 * 返回值：成功处理返回1，失败返回0
 */
int parking_handle(int enter_node,const char *id_card,const char *owner)
{
	char *datetime;
	int stop;
	
	if (LIFT_BAR != parking_check(id_card))
	{
		return 0;
	}
	
	// 入库，写入入库时间
	datetime = get_curr_time();
	if(	record_start_time(id_card,datetime) == 0)
	{
		printf("Error: record start time failed.\n");
		return 0;
	}

	// 停车诱导
	stop = find_parking_place(enter_node);
	if(stop < 0 || stop > 8)
	{
		printf("Error: guidence computes failed.\n");
		return 0;
	}

	// 发布建议停车区域stop
	printf("suggest parking: %s\n",area_node_id(stop));
	send_msg(LED,msgCard_recv.pos,stop);

	return 1;
}
/**
 * 功能：出库处理过程
 * 参数：exit_node - 出口位置
 * 		 id_card - 车主所持卡ID
 * 返回值：处理成功返回1，失败为0
 */
int leave_handle(int exit_node,const char *id_card)
{
	char start_time[100];
	char *stop_time = NULL;
	long dhour;
	long fee;
	int res;
	
	
	// 是否为合法用户
	if(is_valid_user(id_card) == 0)
	{
		printf("Msg: unvalid user.\n");
		logmaker("Msg: unvalid user.");
		return 1;
	}

	// 检车是否有停车记录
	res = query_start_time(id_card,start_time);
	if(res == 0)
	{
		printf("Warning:No parking records.\n");
		logmaker("Warning:No parking records");
		return 1;
	}
	else if(res == -1)
	{
		printf("Error: Datebases inner error.\n");
		logmaker("Error: Database inner error.");
		return 1;
	}

	// 写入出库时间
	stop_time = get_curr_time();

	res = record_stop_time(id_card,start_time,stop_time);
	if(res == 0)
	{
		printf("Error: record exit time failed.\n");
		logmaker("Error: record exit time failed.");
		return 0;
	}
	else if(res == -1)
	{
		printf("Error: Datebases inner error.\n");
		logmaker("Error: Datebases inner error.");
		return 0;
	}

	// 计算停车费用
	dhour = diff_time_hour(start_time,stop_time);
	fee = dhour * FEE_PER_HOUR;
	sprintf(msgCard_send.id_or_time,"%ld",dhour);
	sprintf(msgCard_send.owner_or_fee,"%ld",fee);
	
	printf("Msg: Car leaving.\n");
	logmaker("Msg: Car leaving.");
	printf("Parking time(hour): %ld\n",dhour);
	printf("Parking fee(RMB): %ld\n",fee);

	// 发布停车费用信息
	send_msg(FEE,msgCard_recv.pos,0);

	return 1;
}


void init()
{
	udp();
}
void logmaker(const char *string)
{
	FILE *fp;
	time_t now;
	struct tm *nowtime;
	char strtime[20];
	char *path = NULL;

	now = time(NULL);
	nowtime = localtime(&now);
	sprintf(strtime,"%04d-%02d-%02d.log",nowtime->tm_year+1900,nowtime->tm_mon+1,nowtime->tm_mday);
	path = (char *)malloc(sizeof(char)*(strlen(log_path)+strlen(strtime)+1));
	sprintf(path,"%s%s",log_path,strtime);

	fp = fopen(path,"a+");
	if(!fp)
	{
		printf("Error: cannot open the log file.\n");
		return;
	}
	
	fprintf(fp,"%s %s\n",get_curr_time(),string);

	fclose(fp);
	
	return;
}
int main()
{
	char path_parking_data[] = "./parking_lots.txt"; 
	char log_records[200];
	int row,cor;
	int pos,action;

	// 初始化
	init();
	set_parking_map(path_parking_data,1);
	
	sprintf(log_records,"Server start ...");
	printf("%s %s\n",get_curr_time(),log_records);
	logmaker(log_records);
	
	update_area_node();
	save_area_node();

	while(1)
	{
		int ret = recv_msg();
//		print_area_node(area_node,9);
//		print_parking_map(parking_map,6,60);
		switch(ret)
		{
			case CAP:	/*收到来自超声波采集端的信息 */
				{
					if(msg_recv.pos >= 0 && msg_recv.pos < 360)
					{
						row = msg_recv.pos / 60;
						cor = msg_recv.pos % 60;
						set_parking_map_node(msg_recv.action,row,cor);
						update_area_node();
						save_area_node();
						if(msg_recv.action == 1)
						{
							sprintf(log_records,"%s Msg: Detected parking at pos %d.",get_curr_time(),msg_recv.pos);
							printf("%s\n",log_records);
							logmaker(log_records);
						}
						else if(msg_recv.action == 0)
						{
							sprintf(log_records,"%s Msg: Detected leaving at pos %d.",get_curr_time(),msg_recv.pos);
							printf("%s\n",log_records);
							logmaker(log_records);
						}
					}
					
					//广播信息至 LED 和 CLIENT
					pos = get_area_node(msg_recv.pos);
					action = get_left_in_area_node(pos);
					
					send_msg(LED,pos,action);
					send_msg(CLIENT,msg_recv.pos,msg_recv.action);
					break;
				}		
			case CARD:	/*收到入库出库信息*/
				{
					if ( I1 == msgCard_recv.pos || I2 == msgCard_recv.pos ) /*入库信息*/
					{
						sprintf(log_records,"New coming at %s,id is %s,owner is %s",node_id(msgCard_recv.pos),msgCard_recv.id_or_time,msgCard_recv.owner_or_fee);
						printf("%s %s\n",get_curr_time(),log_records);
						logmaker(log_records);

						parking_handle(msgCard_recv.pos, msgCard_recv.id_or_time, msgCard_recv.owner_or_fee);
					}
					else if ( O1 == msgCard_recv.pos || O2 == msgCard_recv.pos ) /* 出库信息 */
					{
						sprintf(log_records,"Leaving at %s,id is %s,owner is %s",node_id(msgCard_recv.pos),msgCard_recv.id_or_time,msgCard_recv.owner_or_fee);
						printf("%s %s\n",get_curr_time(),log_records);
						logmaker(log_records);

						leave_handle(msgCard_recv.pos,msgCard_recv.id_or_time);
					}
					break;
				}
		}
	}
}
