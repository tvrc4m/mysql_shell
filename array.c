#include "array.h"

int in_array(char *v,const char **array,int count){
	for (int i = 0; i < count; ++i)
	{
		if(!strcmp(array[i],v)) return 1;
	}
	return 0;
}

int array_push(char *v,char **array,int count){
	for (int i = 0; i < count; ++i)
	{
		if(!strlen(array[i])){
			array[i]=v;
			return 1;
		}
	}
	return 0;
}

int array_compare(char **array1,char **array2){
	unsigned int len1=count(array1),len2=count(array2);
	if(len2!=len1) return -1;
	for (int i = 0; i < len1; ++i)
	{
		if(strcmp(array1[i],array2[i])) return -1;
	}
	return 0;
}

char *implode(const char *glue,const char **array,char *str){
	while(*array){
		if(strlen(str)) strcat(str,glue);
		printf("%s\n", *array);
		strcat(str,*array);
		array++;
	}
	return str;
}