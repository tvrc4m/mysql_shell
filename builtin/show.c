#include "model.h"
#include "db.h"
#include "array.h"


const char* about_show_usage="about show --(tables,columns,variables,create)  <options>";

static int check_cmd_args(int argc,const char **argv){
	// if(!in_array("--table",argv,argc) && !in_array("-t",argv,argc)){
	// 	usage(about_show_usage);
	// }
	return 1;
}

static int has_cmd_args(const char **argv,const char *arg){
	while(*argv){
		if(!start_with(*argv,"--") && !strcmp(*argv+2,arg)){
			return 1;
		}
		argv++;
	}
	return 0;
}

int cmd_show(int argc,const char **argv){
	// check necessary param
	check_cmd_args(argc,argv);

	st_show_sql *stsql=(st_show_sql*)malloc(sizeof(st_show_sql));

	char type[50];
	char like[50];
	const char *table="";
	char sql[125];
	int show_header_bottom=1;

	while(*argv){
		if(!strcmp(*argv,"--tables") || !strcmp(*argv,"-t")){
			stsql->type="TABLES";
			argv++;
		}else if(!strcmp(*argv,"--columns") || !strcmp(*argv,"-c")){
			const char *t="COLUMNS";
			argv++;
			if(!*argv || !start_with(*argv,"--")){
				usage("about show --[columns|c] table-name");
			}
			sprintf(type,"%s FROM %s%s",t,DBPRE,*argv);
			stsql->type=type;
			argv++;
		}else if(!strcmp(*argv,"--create")){
			const char *t=*argv+2;
			argv++;
			if(!start_with(*argv,"--")){
				usage("about show --create table-name");
			}
			sprintf(type,"%s TABLE %s%s",t,DBPRE,*argv);
			stsql->type=type;
			argv++;
			show_header_bottom=0;
		}else if(!strcmp(*argv,"--variables") || !strcmp(*argv,"-v")){
			stsql->type="VARIABLES";
			argv++;
		}else if(!strcmp(*argv,"--index") || !strcmp(*argv,"-i")){
			const char *t="INDEXES";
			argv++;
			if(!start_with(*argv,"--")){
				usage("about show --columns table-name");
			}
			sprintf(type,"%s FROM %s%s",t,DBPRE,*argv);
			stsql->type=type;
			argv++;
		}else if(!strcmp(*argv,"--process") || !strcmp(*argv,"-p")){
			const char *t="PROCESSLIST";
			argv++;
			if(*argv){
				if(!strcmp("--full",*argv) || !strcmp("-f",*argv)){
					sprintf(type,"FULL %s",t);
				}
				argv++;
			}else{
				sprintf(type,"%s",t);
			}
			stsql->type=type;
		}else if(!strcmp(*argv,"--grep") || !strcmp(*argv,"-g")){
			argv++;
			cat_sql_like(*argv,like);
			argv++;
		}else{
			argv++;
		}
	}
	stsql->table=table;
	stsql->like=like;
	parse_show_sql(stsql,sql);
	printf("%s\n", sql);
	MYSQL_RES *res=select(sql);
	print_sql_result(res,show_header_bottom);
	free_res(res);
	return 0;
}