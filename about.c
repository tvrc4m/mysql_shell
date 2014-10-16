#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builtin.h"
#include "common.h"
#include "array.h"

const char* about_usage_string =
	"about [--version] [--help] [-C <path>] [-c name=value]\n"
	"           <command> [<args>]";

builtin_cmd cmds[]={
	{"update",cmd_update},
	{"select",cmd_select},
	{"insert",cmd_insert},
	{"delete",cmd_delete},
	{"log",cmd_log},
	{"show",cmd_show},
};

int builtin_cmd_count=count(cmds);

int is_builtin(const char *cmd){
	for (int i = 0; i < builtin_cmd_count; ++i)
	{
		if(!strcmp(cmd,cmds[i].cmd)) return 1;
	}
	return 0;
}

int run_builtin(int argc,const char** argv,const builtin_cmd cmd){

	return cmd.fn(argc-1,argv+1);
}

static int handle_builtin(int argc,const char** argv){
	for (int i = 0; i < builtin_cmd_count; ++i)
	{
		if(!strcmp(cmds[i].cmd,*argv)){
			exit(run_builtin(argc-1,argv++,cmds[i]));
		}
	}
	return 1;
}

static int handle_options(const char* argv[]){

	return 1;
}

int main(int argc,char** av){

	const char** argv=(const char**)av;

	if(argc==1){
		usage(about_usage_string);
	}

	if(is_builtin(argv[1])){
		
		handle_options(argv);
		handle_builtin(argc-1,argv+1);
	}
	return 0;
}