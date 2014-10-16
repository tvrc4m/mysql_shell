#include "db.h"

struct st_select_sql{
	const char *select;
	const char *from;
	const char *where;
	const char *limit;
	const char *order;
	const char *group;
};

typedef struct st_select_sql st_select_sql;

struct st_update_sql{
	const char *from;
	const char *set;
	const char *where;
};

typedef struct st_update_sql st_update_sql;

struct st_insert_sql{
	const char *from;
	const char *set;
};

typedef struct st_insert_sql st_insert_sql;

struct st_delete_sql{
	const char *from;
	const char *where;
};

typedef struct st_delete_sql st_delete_sql;

// enum en_show_type{
// 	talbes;columns,variables,create
// };

struct st_show_sql{
	// enum en_show_type type;
	const char *type;
	const char *table;
	const char *like;
};

typedef struct st_show_sql st_show_sql;

extern char *cat_sql_where(const char *name,const char *value,char *where);
char *cat_sql_sort(const char *field,const char *sort,char * order);
char *cat_sql_limit(const char *value,char * limit);
char *cat_sql_group(const char *field,const char *having,char * group);
char *cat_sql_set(const char *field,const char *value,char * set);
char *cat_sql_like(const char *search,char* like);
extern char *parse_select_sql(st_select_sql *stsql,char *sql);
extern char *parse_update_sql(st_update_sql *stsql,char *sql);
extern char *parse_insert_sql(st_insert_sql *stsql,char *sql);
extern char *parse_delete_sql(st_delete_sql *stsql,char *sql);
extern char *parse_show_sql(st_show_sql *stsql,char *sql);
extern char *get_primary_key(const char *table);
extern void print_sql_result(MYSQL_RES *res,int print_sql_result);


