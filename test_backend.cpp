/**
 * g++ -g -Wall -lmysqlclient -lpthread mysql_encap.cpp mysql_encap.h conn_pool.cpp conn_pool.h lock.h test_backend.cpp -o test_backend.out
 */

#include "conn_pool.h"
#include "mysql_encap.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DB_NAME "taskdb"
//db连接池
ConnPool* conn_pool;
enum {
	low,
	normal,
	high
};
enum {
	state_doing,
	state_delayed,
	state_deleted
};

void add_user(const char *name,const char *psw);
long get_cur_time();
void add_task(const char*user_name, time_t deadline, const char * description, int priority, int state);

void get_top_n_no_delayed_tasks(const char *user_name, int n) {
	MysqlEncap *sql_conn = conn_pool->GetOneConn();
	char sql[256];
	sprintf(sql, "select * from %s.tbl_task where user_name = '%s' and deadline >= %ld  and task_state != %d order by deadline limit %d;",\
	 							DB_NAME, user_name, get_cur_time(), state_deleted, n);
	sql_conn->ExecuteQuery(sql);
	 while(sql_conn->FetchRow()){
			printf("%s,%s\n",sql_conn->GetField("task_id"), sql_conn->GetField("deadline"));
	 }
	
	conn_pool->ReleaseOneConn(sql_conn);
}

void get_top_n_delayed_tasks(const char *user_name, int n) {
	MysqlEncap *sql_conn = conn_pool->GetOneConn();
	char sql[256];
	sprintf(sql, "select * from %s.tbl_task where user_name = '%s' and deadline < %ld  and task_state != %d order by deadline desc limit %d;",\
	 							DB_NAME, user_name, get_cur_time(), state_deleted, n);
	sql_conn->ExecuteQuery(sql);
	 while(sql_conn->FetchRow()){
			printf("%s,%s\n",sql_conn->GetField("task_id"), sql_conn->GetField("deadline"));
	 }
	
	conn_pool->ReleaseOneConn(sql_conn);
}

int main()
{
	conn_pool = ConnPool::GetInstance();
	
	//add_user("chengshuguang", "21321133");
	//add_task("chengshuguang", 1408551600, "no delayed test", 2, 0);
	get_top_n_delayed_tasks("chengshuguang", 5);
	printf("time:%ld\n", get_cur_time());
	get_top_n_no_delayed_tasks("chengshuguang", 2);
	//MysqlEncap *sql_conn = conn_pool->GetOneConn();
	//sql_conn->ExecuteQuery("select * from taskdb.tbl_user;");

	//conn_pool->ShowStatus();
	
	//conn_pool->ReleaseOneConn(sql_conn);
	//conn_pool->ShowStatus();

	return 0;
}
long get_cur_time() {
	return time(NULL);
}
void add_task(const char*user_name, time_t deadline, const char * description, int priority, int state) {
	MysqlEncap *sql_conn = conn_pool->GetOneConn();
	char sql[256];
	sprintf(sql, "insert into %s.tbl_task values(NULL, '%s', %ld, '%s', %d, %d);", DB_NAME, user_name, deadline,\
	description, priority, state);
	sql_conn->Execute(sql);
	
	conn_pool->ReleaseOneConn(sql_conn);
}
void add_user(const char *name, const char *psw) {
	MysqlEncap *sql_conn = conn_pool->GetOneConn();
	char sql[256];
	sprintf(sql, "insert into %s.tbl_user values(NULL, '%s', '%s');", DB_NAME, name, psw);
	sql_conn->Execute(sql);
	
	conn_pool->ReleaseOneConn(sql_conn);
}
