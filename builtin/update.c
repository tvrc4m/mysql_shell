#include "model.h"
#include "array.h"
#include "log.h"

const char* about_update_usage="about update -t|--table tablename <options>";

static int check_cmd_args(int argc,const char **argv){
	if(!in_array("--table",argv,argc) && !in_array("-t",argv,argc)){
		usage(about_update_usage);
	}
	if(!in_array("--set",argv,argc)){
		usage(about_update_usage);
	}
	return 1;
}

int cmd_update(int argc,const char** argv){

	check_cmd_args(argc,argv);

	st_update_sql *stsql=(st_update_sql*)malloc(sizeof(st_update_sql));

	char sql[1024];
	char *from="";
	char set[512];
	char where[128]="";
	char columns[100]="";

	while(*argv){

		if(!start_with(*argv,"-t") || !start_with(*argv,"--table")){
			argv++;
			from=(char*)*argv;
			argv++;
		}else if(!strcmp(*argv,"--set")){
			argv++;
			while(*argv){
				if(start_with(*argv,"--")==0 || *argv==NULL) break;
				const char *field=*argv;
				if(strlen(columns)) strcat(columns,",");
				strcat(columns,field);
				argv++;
				const char *value=*argv;
				if(value==NULL) usage(about_update_usage);
				cat_sql_set(field,value,set);
				argv++;
			}
		}else if(!start_with(*argv,"--")){
			const char *name=*argv+2;
			if(!strcmp(name,"id"))
				name=get_primary_key(from);
			argv++;
			const char *value=*argv;
			cat_sql_where(name,value,where);
			argv++;
		}else{
			argv++;
		}
	}

	stsql->from=from;
	stsql->set=set;
	stsql->where=where;

	parse_update_sql(stsql,sql);

	puts(sql);

	puts("are you sure update ? (y|n)");

	int confirm=getchar();
	
	if(confirm!=121 && confirm!=89) exit(0);

	query(sql);

	sqllog("update",sql);

	free(stsql);

	stsql=NULL;

	st_select_sql *slsql=(st_select_sql*)malloc(sizeof(st_select_sql));
	slsql->from=from;
	slsql->where=where;
	slsql->select=columns;
	slsql->limit="";
	slsql->group="";
	slsql->order="";

	parse_select_sql(slsql,sql);
	MYSQL_RES *res=select(sql);
	print_sql_result(res,1);
	free_res(res);
	free(slsql);
	return 0;
}