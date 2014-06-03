#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dijkstra.h"
#include "sql.h"

#define FEE_PER_HOUR 5 // 每小时停车费用

/**
 * 功能：检查当前是否还能停车
 * 参数：id_card - 车主所持卡ID
 * 		 emsg - 不能停车的原因
 * 返回值：可以停返回1，emsg指向NULL，不可以停返回0，emsg为详细
 */
int parking_check(const char *id_card,char *emsg)
{
	int can_stop;
	char start_time[100];
	// 车库是否已满
	if(is_full() == 1)
	{// 已满
		emsg = (char *)malloc(sizeof(char)*100);
		strcpy(emsg,"The parking lots is full.");
		return 0;
	}
	
	// 是否是合法用户
	if(is_valid_user(id_card) == 0)
	{// 非法
		emsg = (char *)malloc(sizeof(char)*50);
		strcpy(emsg,"Valid user.");
		return 0;
	}
	
	// 是否已停过车
	if(query_start_time(id_card,start_time) == 1)
	{// 已停过
		emsg = (char *)malloc(sizeof(char)*150);
		sprintf(emsg,"The id has parked at %s.",start_time);		
		return 0;
	}
	
	emsg = NULL;
	return 1;
}

/**
 * 功能：发出抬杆消息
 * 参数：enter_node - 入库位置
 * 返回值：无
 */
void send_msg_lift_bar(int enter_node)
{
	
}
/**
 * 功能：发出建议停车位置
 * 参数：enter_node - 入库位置
 * 		 stop - 建议停车区域
 * 返回值：无
 */
void send_msg_suggest_parking_area(int enter_node,int stop)
{
	
}
/**
 * 功能：发出停车费用信息
 * 参数: exit_node - 出口位置
 * 		 parking_time - 停车时间
 * 		 fee - 停车费用
 * 返回值：无
 */
void send_msg_parking_fee(int enter_node,int parking_time,int fee)
{
	
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
	char *emsg;
	char *datetime;
	int stop;
	if(parking_check(id_card,emsg) == 0)
	{
		printf("Warning: %s\n",emsg);
		return 1;
	}

	// 抬杆放行
	lift_bar();

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
	set_suggest_parking_area(enter_node,stop);

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
	show_parking_fee(exit_node,dhour,fee);

	return 1;
}
/**
 * 功能：跟新所有区域显示牌的空车位信息
 * 参数：keep_minutes - 该过程持续时长
 * 		 interval - 刷新间隔时间
 * 返回值：无
 */
void update_area_info_handle(int keep_minutes,int interval_seconds)
{
	
}

void init()
{
	//set_parking_map(path_parking_data,1);
}
int main()
{
	char path_parking_data[] = "./data/parking_lots.txt";
	int stop;

	// 初始化
	
	// 系统自检
	
	// 等待客户端请求
	
		// 创建新线程处理请求

	// 重复
}
