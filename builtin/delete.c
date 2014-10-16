#include "model.h"
#include "array.h"
#include "log.h"

const char *about_delete_usage="about insert -t|--table table-name  <options>";

static int check_cmd_args(int argc,const char **argv){
	if(!in_array("--table",argv,argc) && !in_array("-t",argv,argc)){
		usage(about_delete_usage);
	}
	return 1;
}

int cmd_delete(int argc,const char **argv){

	check_cmd_args(argc,argv);

	st_delete_sql *stsql=(st_delete_sql*)malloc(sizeof(st_delete_sql));

	char sql[1024];
	char *from="";
	char where[24]={""};

	while(*argv){

		if(!start_with(*argv,"-t") || !start_with(*argv,"--table")){
			argv++;
			from=(char*)*argv;
			argv++;
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

	st_select_sql *slsql=(st_select_sql*)malloc(sizeof(st_select_sql));
	slsql->from=from;
	slsql->where=where;
	slsql->select="*";
	slsql->limit="";
	slsql->group="";
	slsql->order="";

	parse_select_sql(slsql,sql);
	MYSQL_RES *res=select(sql);
	print_sql_result(res,1);
	free_res(res);
	free(slsql);

	stsql->from=from;
	stsql->where=where;

	parse_delete_sql(stsql,sql);

	puts(sql);

	puts("are you sure delete ? (y|n)");

	int confirm=getchar();
	
	if(confirm!=121 && confirm!=89) exit(0);

	int id=insert(sql);

	sqllog("insert",sql);

	free(stsql);

	stsql=NULL;
	
	return 0;
}

