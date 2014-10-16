#include "log.h"
#include "model.h"
#include "time.h"

static st_sql_log **logs;

int file_exist(char* filename){
	FILE *fp=fopen(filename,"r");
	if(fp){
		fclose(fp);
		return 1;
	}
	return 0;
}

st_sql_log *get_date_log(char *dt){
	char f[100];
	sprintf(f,"%s%s%s",LOG_PATH,dt,LOG_EXT);
	if(!file_exist(f)){
		return NULL;
	}
	FILE *fp=fopen(f,"r");
	int ch;
	int rows=0;
	while((ch=fgetc(fp))!= EOF){
		if(ch==10){ // \n
			rows++;
		}
	}
	if(rows==0) return NULL;
	st_sql_log *logs=(st_sql_log*)calloc(sizeof(st_sql_log),rows);
	// char *sqltime=(char*)malloc(sizeof(char)*30);
	// char *sql=(char*)malloc(sizeof(char)*1024);
	char sqltime[64]={""};
	char sql[1024]={""};
	int is_sqltime=1;
	int is_sql=0;
	rewind(fp);
	int row=0,i=0,j=0;
	while((ch=fgetc(fp))!=EOF){
		if(is_sqltime){ // \t
			if(ch==9){
				is_sqltime=0;
				is_sql=1;
				i=0;
				printf("%s\t", sqltime);
				continue;
			}
			sqltime[i]=ch;
			i++;
		}
		if(is_sql){
			if(ch==10){
				// logs[row]=(st_sql_log){sqltime,sql};
				printf("%s\n", sql);
				// memset(sqltime,'',64);
				// memset(sql,'',1024);
				is_sqltime=1;
				is_sql=0;
				j=0;
				row++;
				continue;	
			}
			
			sql[j]=ch;
			j++;
		}
	}
	return NULL;
}

void sqllog(char *type,char *sql){
	char filepath[128]="";
	time_t now=time(NULL);
	struct tm *tmptr=gmtime(&now);
	int month=tmptr->tm_mon;
	sprintf(filepath,"%s%d%d%d%s",LOG_PATH,tmptr->tm_year+1900,tmptr->tm_mon+1,tmptr->tm_mday,LOG_EXT);
	char logstr[1024];
	sprintf(logstr,"%d-%d-%d %d:%d:%d\t%s",tmptr->tm_year+1900,tmptr->tm_mon+1,tmptr->tm_mday,tmptr->tm_hour,tmptr->tm_min,tmptr->tm_sec,sql);
	FILE *fp=fopen(filepath,"a+");
	fprintf(fp, "%s\n", logstr);
}

static char *get_file_name(char *filepath){
	time_t now=time(NULL);
	struct tm *tmptr=gmtime(&now);
	int month=tmptr->tm_mon;
	// char *str_month=month<10?
	sprintf(filepath,"%s%d%d%d%s",LOG_PATH,tmptr->tm_year+1900,tmptr->tm_mon+1,tmptr->tm_mday,LOG_EXT);
	return filepath;
}

static char *get_current_date(){
	time_t now=time(NULL);
	struct tm *tmptr=gmtime(&now);
	char *current=malloc(sizeof(char)*9);
	sprintf(current,"%d%d%d",tmptr->tm_year+1900,tmptr->tm_mon+1,tmptr->tm_mday);
	return current;
}

static int check_cmd_args(int argc,const char **argv){
	return 1;
}

int cmd_log(int argc,const char** argv){
	// check necessary param
	check_cmd_args(argc,argv);

	char *current="";

	while(*argv){
		if(!strcmp("-t",*argv)){
			argv++;
			current=(char*)*argv;
			argv++;
		}else{
			argv++;
		}
	}

	if(!strlen(current)) current=get_current_date();

	st_sql_log *logs=get_date_log(current);
	// while(*logs){
	// 	printf("%s\n", (*logs).sqltime);
	// }
	return 1;
}