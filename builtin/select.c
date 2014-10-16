#include "model.h"
#include "db.h"
#include "array.h"


const char* about_select_usage="about select -t|--table table-name --select  <options>";

static int check_cmd_args(int argc,const char **argv){
	if(!in_array("--table",argv,argc) && !in_array("-t",argv,argc)){
		usage(about_select_usage);
	}
	return 1;
}

int has_cmd_args(const char **argv,const char *arg){
	while(*argv){
		if(!start_with(*argv,"--") && !strcmp(*argv+2,arg)){
			return 1;
		}
		argv++;
	}
	return 0;
}

int cmd_select(int argc,const char **argv){
	// check necessary param
	check_cmd_args(argc,argv);

	st_select_sql *stsql=(st_select_sql*)malloc(sizeof(st_select_sql));
	stsql->select="*";
	stsql->limit=" LIMIT 20";
	char *from="";
	char where[256]={""};
	char order[128]={""};
	char limit[16]={""};
	char group[64]={""};

	int has_limit=has_cmd_args(argv,"limit");
	while(*argv){
		if(!strcmp(*argv,"--table") || !strcmp(*argv,"-t")){
			argv++;
			from=(char*)*argv;
			argv++;
		}else if(!strcmp(*argv,"--select")){
			argv++;
			stsql->select=*argv;
			argv++;
		}else if(!strcmp(*argv,"--limit")){
			argv++;
			stsql->limit=cat_sql_limit(*argv,limit);
			argv++;
		}else if(!strcmp(*argv,"--order")){
			argv++;
			const char *field=*argv;
			argv++;
			if(*argv==NULL){
				cat_sql_sort(field,"ASC",order);
			}else{
				if(start_with(*argv,"--")){
					cat_sql_sort(field,*argv,order);
				}else{
					cat_sql_sort(field,"ASC",order);
				}
			}
		}else if(!strcmp(*argv,"--group")){
			argv++;
			stsql->group=cat_sql_group(*argv,"",group);
			argv++;
			if(has_limit==0){
				stsql->limit="";
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
	stsql->where=where;
	stsql->order=order;
	stsql->group=group;
	char sql[512];
	parse_select_sql(stsql,sql);
	MYSQL_RES *res=select(sql);
	print_sql_result(res,1);
	free_res(res);
	return 0;
}