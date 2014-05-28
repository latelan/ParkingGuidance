#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dijkstra.h"
#include "sql.h"

#define FEE_PER_HOUR 5 // 每小时停车费用

/* 停车检测信息，赋值给发送包的action */
#define FULL 0
#define UNVALID 1
#define EXIST 2
#define ALLOW 3


/**
 * 功能：检查当前是否还能停车
 * 参数：id_card - 车主所持卡ID
 * 		 emsg - 不能停车的原因
 * 返回值：可以停返回1，emsg指向NULL，不可以停返回0，emsg为详细
 */
int parking_check(const char *id_card)
{
	int can_stop;
	char start_time[100];
	// 车库是否已满
	if(is_full() == 1)
	{
		send_msg(BAR,msg_recv.pos,FULL);
		return FULL;
	}
	
	// 是否是合法用户
	if(is_valid_user(id_card) == 0)
	{
		send_msg(BAR,msg_recv.pos,UNVALID);
		return UNVALID;
	}
	
	// 是否已停过车
	if(query_start_time(id_card,start_time) == 1)
	{	
		send_msg(BAR,msg_recv.pos,EXIST);	
		return EXIST;
	}
	//可以停车
	send_msg(BAR,msg_recv.pos,ALLOW);
	return ALLOW;
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

	if (ALLOW != parking_check(msgCard.id))
	{
		printf("Warning:can not parking\n");
		return;
	}

	// 入库，写入入库时间
	datetime = get_curr_time();
	if(	record_start_time(id_card,datetime) == 0)
	{
		printf("Error: record start time failed.\n");
		return 0;
	}

	// 停车诱导
	if(get_final_stop(enter_node,&stop) == 0)
	{
		printf("Error: guidence computes failed.\n");
		return 0;
	}

	// 发布建议停车区域stop
	send_msg(LED,msg_recv.pos,stop);

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
	// 检车是否有停车记录
	res = query_start_time(id_card,start_time);
	if(res == 0)
	{
		printf("Warning:No parking records.\n");
		return 1;
	}
	else if(res == -1)
	{
		printf("Error: Datebases inner error.\n");
		return 1;
	}

	// 写入出库时间
	stop_time = get_curr_time();

	res = record_stop_time(id_card,start_time,stop_time);
	if(res == 0)
	{
		printf("Error: record exit time failed.\n");
		return 0;
	}
	else if(res == -1)
	{
		printf("Error: Datebases inner error.\n");
		return 0;
	}

	// 计算停车费用
	dhour = diff_time_hour(start_time,stop_time);
	fee = dhour * FEE_PER_HOUR;

	// 发布停车费用信息
	send_msg(LED,msg_recv.pos,0);

	return 1;
}

void init()
{
	set_parking_map(path_parking_data,1);
}
int main()
{
	char path_parking_data[] = "./data/parking_lots.txt";
	int stop;

	// 初始化
	init();
	// 系统自检
	
	while(1)
	{
		int ret = recv_msg();
		switch(ret)
		{
			case CAP:	/*收到来自超声波采集端的信息 */
				{
					//更新数组
					// TODO

					//广播信息至 LED 和 CLIENT
					send_msg(LED,msg_recv.pos,msg_recv.action);
					send_msg(CLIENT,msg_recv.pos,msg_recv.action);
				};break;
			case CARD:	/*收到入库/出库信息*/
				{
					if ( 1 == msgCard.pos || 2 == msgCard.pos ) /*入库信息*/
					{
						if (ALLOW == parking_check(msgCard.id))
						{
							parking_handle(msgCard.pos,msgCard.id,msgCard.owner)
						}
						
					}
					else if ( 3 == msgCard.pos || 4 == msgCard.pos ) /* 出库信息 */
					{
						leave_handle(msgCard.pos,msgCard.id)
					}
				};break;
		}
	}
}