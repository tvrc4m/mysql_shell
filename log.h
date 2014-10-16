#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define LOG_PATH "/imaginato/about/shell/log/"

#define LOG_EXT ".log"

struct st_sql_log{
	char *sqltime;
	char *sql;
};

typedef struct st_sql_log st_sql_log;

static char *get_file_name();

extern void sqllog(char *type,char *sql);

extern st_sql_log *get_date_log(char *dt);

extern int file_exist(char* filename);

// extern void 

