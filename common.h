#ifndef ABOUT_COMMON_H
#define ABOUT_COMMON_H

#define count(x) (sizeof(x)/sizeof(x[0]))

void usage(const char* usage_string);

extern int start_with(const char* str,const char* pre);
extern int end_with(const char *str,const char *suffix);
extern char *strpad(char *str,int total,const char *padding);
extern char *strtoupper(const char *str);

#endif