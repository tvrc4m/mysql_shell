#include "db.h"
#include "common.h"
int connect(){
	mysql_init(&mysql);
	if(mysql_real_connect(&mysql,DBHOST,DBUSER,DBPWD,DBNAME,0,NULL,0)==NULL){
		perror("MYSQL CONNECT ERROR");
		printf("%s\n", mysql_error(&mysql));
		return -1;
	}
	return 0;
}

MYSQL_RES *select(const char *sql){
	if(!connect()){
		int status=mysql_query(&mysql,sql);
		if(!status){
			MYSQL_RES *res=mysql_store_result(&mysql);
			return res;
		}else{
			perror(sql);
			printf("%s\n", mysql_error(&mysql));
		}
	}
	return NULL;
}


dict_t **find(const char *sql){
	if(!connect()){
		int status=mysql_query(&mysql,sql);
		if(!status){
			MYSQL_RES *res=mysql_store_result(&mysql);
			int fc=mysql_field_count(&mysql);
			int rc=mysql_num_rows(res);
			dict_t **result=(dict_t **)calloc(sizeof(dict_t),rc*fc);
			if(res){
				MYSQL_FIELD *fields=mysql_fetch_fields(res);
				for (int i = 0; i < rc; ++i){
					dict_t *dict=(dict_t*)malloc(sizeof(dict_t));
					dict_init(dict);
					MYSQL_ROW row=mysql_fetch_row(res);
					for(int j=0 ; j<fc ; j++){
						dict_set(dict,fields[j].name,row[j]);
						// *(result+i*fc+j)=dict;
					}
					result[i]=dict;
				}
			}

			mysql_free_result(res);
			mysql_close(&mysql);
			// free(rows);
			return result;
		}else{
			perror(sql);
			printf("%s\n", mysql_error(&mysql));
		}
	}
	return NULL;
}

// st_sql_result get(const char *sql){
// 	if(!connect()){
// 		int status=mysql_query(&mysql,sql);
// 		MYSQL_ROW row;
// 		if(!status){
// 			MYSQL_RES *res=mysql_store_result(&mysql);
// 			if(res){

// 				row=mysql_fetch_row(res);
// 				mysql_free_result(res);
// 				mysql_close(&mysql);
// 				return row;
// 			}
// 		}else{
// 			perror(sql);
// 			printf("%s\n", mysql_error(&mysql));
// 		}
// 	}
// 	return NULL;
// }

int query(const char *sql){
	if(!connect()){
		int status=mysql_query(&mysql,sql);
		if(!status){
			return mysql_affected_rows(&mysql);
		}
		printf("%s\n", mysql_error(&mysql));
		mysql_close(&mysql);
	}
	return -1;
}

int insert(const char *sql){
	if(!connect()){
		int status=mysql_query(&mysql,sql);
		if(!status){
			return mysql_insert_id(&mysql);
		}
		printf("%s\n", mysql_error(&mysql));
		mysql_close(&mysql);
	}
	return -1;
}

void free_res(MYSQL_RES *res){
	mysql_free_result(res);
}

void close(MYSQL *mysql){
	mysql_close(mysql);
}