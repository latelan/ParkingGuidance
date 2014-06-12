#ifndef _SQL_H_
#define _SQL_H_

#include "/usr/include/mysql/mysql.h"

/* 创建数据库连接 */
MYSQL *open(const char *host,const char *user,const char *password,const char *database);

/* 检查id_card是否为合法用户 */
int is_valid_user(const char *id_card);

/* 记录入库时间 */
int record_start_time(const char *id_card,const char *start_time);

/* 记录出库时间 */
int record_stop_time(const char *id_card,const char *start_time,const char *stop_time);

/* 查询id_card的车主姓名 */
char *query_owner(const char *id_card);

/* 查询入库时间 */
int query_start_time(const char *id_card, char *start_time);

/* 查询出库时间 */
int query_stop_time(const char *id_card,const char *start_time, char *stop_time);

#endif // _SQL_H_
