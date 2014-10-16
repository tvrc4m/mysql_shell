#include <string.h>
#include "common.h"
#include "model.h"
#include "dict.h"

char *cat_sql_where(const char *name,const char *value,char *where){
	char tmp[128];
	sprintf(tmp," %s = '%s'",name,value);
	if(strlen(where)) strcat(where," AND ");
	if(strlen(where)==0) strcat(where," WHERE ");
	return strcat(where,tmp);
}

char *cat_sql_sort(const char *field,const char *sort,char * order){
	char tmp[128];
	sprintf(tmp,"%s %s",field,sort);
	if(strlen(order)) strcat(order,",");
	if(strlen(order)==0) strcat(order," ORDER BY ");
	return strcat(order,tmp);
}

char *cat_sql_group(const char *field,const char *having,char * group){
	sprintf(group," GROUP BY %s ",field);
	return group;
}

char *cat_sql_limit(const char *value,char * limit){
	sprintf(limit," LIMIT %s",value);
	return limit;
}

char *cat_sql_set(const char *field,const char *value,char * set){
	char tmp[512];
	sprintf(tmp," %s='%s' ",field,value);
	if(strlen(set)) strcat(set,",");
	return strcat(set,tmp);
}

char *cat_sql_like(const char *search,char* like){
	sprintf(like," LIKE '%s%s%s' ","%",search,"%");
	return like;
}

char *cat_sql_columns(const char *search,char* like){
	sprintf(like," FROM %s",search);
	return like;
}

char *parse_select_sql(st_select_sql *stsql,char *sql){
	sprintf(sql,"SELECT %s FROM %s%s%s%s%s%s",stsql->select,DBPRE,stsql->from,stsql->where,stsql->group,stsql->order,stsql->limit);
	printf("%s\n", sql);
	return 0;
}

char *parse_update_sql(st_update_sql *stsql,char *sql){
	sprintf(sql,"UPDATE %s%s SET %s%s",DBPRE,stsql->from,stsql->set,stsql->where);
	return sql;
}

char *parse_insert_sql(st_insert_sql *stsql,char *sql){
	sprintf(sql,"INSERT INTO %s%s SET %s",DBPRE,stsql->from,stsql->set);
	return sql;
}

char *parse_delete_sql(st_delete_sql *stsql,char *sql){
	sprintf(sql,"DELETE FROM %s%s %s",DBPRE,stsql->from,stsql->where);
	return sql;
}

char *parse_show_sql(st_show_sql *stsql,char *sql){
	sprintf(sql,"SHOW %s %s%s",strtoupper(stsql->type),strtoupper(stsql->table),stsql->like);
	return sql;
}

char *get_primary_key(const char *table){
	char sql[256];
	sprintf(sql,"SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE table_schema='%s' and table_name='%s%s' and extra='auto_increment'",DBNAME,DBPRE,table);
	MYSQL_RES *res=select(sql);
	if(!res) return "";
	return res->data_cursor->data[0];
}

void print_sql_result(MYSQL_RES *res,int show_header_bottom){
	unsigned int field_count=res->field_count;
	my_ulonglong row_count=res->row_count;
	if(row_count==0){
		printf("%s\n", "no rows");
		exit(0);
	}
	char *columns[row_count+4][field_count];
	int maxlen[field_count];
	for (int i = 0; i < field_count; ++i){
		maxlen[i]=strlen(res->fields[i].name);
		columns[1][i]=res->fields[i].name;
	}
	int j=3;
	MYSQL_ROWS *row=res->data_cursor;
	do{
		for (int i = 0; i < field_count; ++i){
			if(row->data[i]){
				if(maxlen[i]<strlen(row->data[i])) maxlen[i]=strlen(row->data[i]);	
				columns[j][i]=row->data[i];
			}else{
				columns[j][i]="";
			}
		}
		j++;
	}while((row=row->next)!=NULL);

	for (int i = 0; i < row_count+4; ++i){
		for (int f = 0; f < field_count; ++f){
			if(i==0 || i==2 || i==row_count+3){
				if(!show_header_bottom) continue;
				char *top=(char*)malloc(sizeof(char)*maxlen[f]);
				strpad(top,maxlen[f],"-");
				printf("+ %s ", top);
			}else{
				if(i==1 && !show_header_bottom) continue;
				char * tmpvalue=(char*)malloc(sizeof(char)*maxlen[f]);
				strcat(tmpvalue,columns[i][f]);
				strpad(tmpvalue,maxlen[f]," ");
				printf("| %s ", tmpvalue);
			}
		}
		if(i==0 || i==2 || i==row_count+3){
			if(show_header_bottom)
				printf("%s\n", "+");
		}else
			printf("%s\n", "|");
	}
}