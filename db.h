#ifndef DB_H
#define DB_H
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "mysql.h"
#include "dict.h"

MYSQL mysql;
MYSQL_ROW row;
MYSQL_FIELD *field;

#define DBHOST "127.0.0.1"
#define DBPORT 3360
#define DBUSER "about"
#define DBPWD  "about"
#define DBNAME "about"
#define DBPRE  "oc_"

struct st_sql_cell{
	char *field;
	char *value;
};

typedef struct st_sql_cell **st_sql_cell;

struct st_sql_result{
	st_sql_cell cell;
	int fields;
	int rows;
};

typedef struct st_sql_result *st_sql_result;


extern int connect();
extern dict_t **find(const char *sql);
extern int query(const char *sql);
extern MYSQL_RES *select(const char *sql);
extern int insert(const char *sql);
extern MYSQL_ROW get(const char *sql);
void free_res(MYSQL_RES *res);
extern void close();
#endif