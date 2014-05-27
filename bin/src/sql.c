#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "/usr/include/mysql/mysql.h"
#include "datetime.h"



#define HOST "localhost"
#define USER "admin"
#define PASSWORD "admin@parking.com"
#define DATABASE "parking"

const char *host = HOST;
const char *user = USER;
const char *password = PASSWORD;
const char *database = DATABASE;
/**
 * 功能：打开数据库
 * 参数：host - 数据库服务主机
 *       user - 用户名
 *       password - 密码
 *       database - 数据库名
 * 返回值：MYSQL 句柄
 */
MYSQL *open(const char *host,const char *user,const char *password,const char *database)
{
	MYSQL *mysql;

	mysql = mysql_init(NULL);
	if(mysql == NULL)
	{
		printf("mysql_init failed!\n");
		return NULL;
	}

	mysql = mysql_real_connect(mysql,host,user,password,database,0,NULL,0);
	if(mysql == 0)
	{
		printf("mysql_real connect failed!\n");
		return NULL;
	}

	return mysql;
}
/**
 * 功能：检查是否为合法用户
 * 参数：id_card - 车主所持卡id
 * 		 user - 车主姓名
 * 返回值：是合法用户返回1，非法返回0，数据库内部出错返回-1
 */
int is_valid_user(const char *id_card,const char *user)
{
	MYSQL *mysql;
	MYSQL_RES *result;
	MYSQL_ROW row;
	char query_str[50];
	int affect_rows = 0;
	int valid_tag = 0;
	mysql = open(host,user,password,database);
	if(mysql == NULL)
	{
		printf("Connect db failed.\n");
		return -1;
	}
	sprintf(query_str,"select owner from user where id_num = '%s'",id_card);

	if(mysql_query(mysql,query_str))
	{
		return -1;
	}
	
	result = mysql_use_result(mysql);
	if((row = mysql_fetch_row(result)) != NULL)
	{
		affect_rows = mysql_num_rows(result);
		if(affect_rows > 0 && strcmp(row[0],user) == 0)
		{
			valid_tag = 1;
		}
	}	
	mysql_free_result(result);
	mysql_close(mysql);
	
	return valid_tag;
}
/**
 * 功能：向数据库写入入库时间
 * 参数：id_card - 车主所持卡id
 * 		 start_time - 入库时间
 * 返回值：写入成功返回1，失败返回0，数据库内部出错返回-1
 */
int record_start_time(const char *id_card,const char *start_time)
{
	MYSQL *mysql;
	MYSQL_RES *result;
	char query_str[150];
	int affect_rows;
	mysql = open(host,user,password,database);

	sprintf(query_str,"insert into parking_record(id_num,in_time) values ('%s','%s')",id_card,start_time);

	if(mysql_query(mysql,query_str))
	{
		return -1; // errors
	}

	affect_rows = mysql_affected_rows(mysql);
	mysql_close(mysql);
	if(affect_rows > 0)
	{
		return 1; // insert success
	}
	return 0;

}
/**
 * 功能：查询车主信息
 * 参数；id_card - 车主所持卡ID
 * 返回值：成功查询返回owner，出错返回NULL
 */
char *query_owner(const char *id_card)
{
	MYSQL *mysql = NULL;
	MYSQL_RES *result;
	MYSQL_ROW row;
	char query_str[100];
	mysql = open(host,user,password,database);
	if(mysql == NULL)
	{
		return NULL;
	}
	
	sprintf(query_str,"select owner from user where id_num = '%s'",id_card);

	if(mysql_query(mysql,query_str))
	{
		return NULL;
	}

	result = mysql_use_result(mysql);

	if((row = mysql_fetch_row(result)) != NULL)
	{
		return row[0];
	}

	return NULL;
}
/**
 * 功能：查询入库时间
 * 参数：id_card - 车主所持卡ID
 * 		 start_time - 存放查询结果
 * 返回值：成功查询返回时间，出错返回0，数据内部出错返回-1
 */
int query_start_time(const char *id_card, char *start_time)
{
	MYSQL *mysql = NULL;
	MYSQL_RES *result;
	MYSQL_ROW row;
	char query_str[200];
	
	mysql = open(host,user,password,database);
	if(mysql == NULL)
	{
		return -1;
	}
	
	sprintf(query_str,"select in_time  from parking_record where id_num = '%s' and ( out_time is null or out_time = '')",id_card);

	if(mysql_query(mysql,query_str))
	{
		return -1;
	}

	result = mysql_use_result(mysql);
	if((row = mysql_fetch_row(result)) != NULL)
	{
		strcpy(start_time,row[0]);
		return 1;
	}

	return 0;

}
/**
 * 功能：查询id_card和start_time所对应的stop_time
 * 参数：id_card - 车主所持卡ID
 * 		 start_time - 车入库时间
 * 		 stop_time  - 车出库时间
 * 返回值：查询结果保存在stop_time中，成功查询返回大于0的值，失败返回0，数据库内部出错返回-1
 */
int query_stop_time(const char *id_card, const char *start_time, char *stop_time)
{

}
/**
 * 功能：记录停车出库时间
 * 参数：id_card - 车主所持卡ID
 * 		 stop_time - 出库时间
 * 返回值；成功记录返回大于0的值，失败返回0，数据库内部错误返回-1
 */
int record_stop_time(const char *id_card, const char *start_time, const char *stop_time)
{
	MYSQL *mysql = NULL;
	char query_str[150];
	int affect_rows;

	mysql = open(host,user,password,database);

	sprintf(query_str,"update parking_record set out_time = '%s' where id_num = '%s' and in_time = '%s'",stop_time,id_card,start_time);

	if(mysql_query(mysql,query_str))
	{
		return -1; // errors
	}
	
	affect_rows = mysql_affected_rows(mysql);

	mysql_close(mysql);
	if(affect_rows > 0)
	{
		return affect_rows;
	}

	return 0;
}
/*
int main()
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char exe[50];
	int result = 0;	
	char *cur_time,*stop_time;
	char start_time[50];
	long parkingtime;
	
	// test is_valid_user
	result = is_valid_user("1");
	printf("is_valid_user: %d\n",result);

	// test query_owner
	printf("owner: %s\n",query_owner("1"));

	// test record_start_time
//	cur_time = get_curr_time();
//	printf("record_start_time: %d\n",record_start_time("1",cur_time));

	// test query_start_time
	query_start_time("1",start_time);	
	printf("start_time: %s\n",start_time);

	// test record_stop_time
//	stop_time = get_curr_time();
//	printf("record_stop_time: %d\n",record_stop_time("1",start_time,stop_time));

	// test diff_time
	parkingtime = diff_time_hour(start_time,get_curr_time());
	printf("parkingtime: %ld\n",parkingtime);
	return 0;
}
*/
