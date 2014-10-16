#ifndef BUILTIN_H
#define BUILTIN_H

struct builtin_cmd {
	const char* cmd;
	int (*fn)(int,const char**);
	// int option;
};

typedef struct builtin_cmd builtin_cmd;

extern int is_builtin(const char*);


extern int cmd_update(int argc,const char **argv);
extern int cmd_select(int argc,const char **argv);
extern int cmd_insert(int argc,const char **argv);
extern int cmd_delete(int argc,const char **argv);
extern int cmd_log(int argc,const char **argv);
extern int cmd_show(int argc,const char **argv);


#endif