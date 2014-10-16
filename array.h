#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"


int in_array(char *v,const char **array,int count);

int array_push(char *v,char **array,int count);

int array_compare(char **array1,char **array2);

char *implode(const char *glue,const char **array,char *str);