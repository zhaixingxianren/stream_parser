#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum LogLevel {
	Debug,
	Info,
	Error,
};

extern LogLevel getLevel();
extern LogLevel g_level;

#define Log(level,...)   \
	do {			 	 \
		if(level >= g_level){		 \
			fprintf(stderr," [%s] ",__FUNCTION__); \
			fprintf(stderr,__VA_ARGS__); \
			fprintf(stderr,"\n");   \
		}							\
	}while(0);

