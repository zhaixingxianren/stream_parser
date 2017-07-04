#include <log.h>

LogLevel getLevel(){
	char * lv  =getenv("PARSER_LEVEL");
	if(lv){
		if(strcmp(lv,"Debug") == 0){
			//fprintf(stderr,"level:%s\n",lv);
			return Debug;
		}else if (strcmp(lv,"Info") == 0){
			//fprintf(stderr,"level:%s\n",lv);
			return Info;
		}else if (strcmp(lv,"Error") == 0){
			//fprintf(stderr,"level:%s\n",lv);
			return Error;
		}else{
			//fprintf(stderr,"level:%s\n",lv);
			return Info;
		}
	}else return Info; //default info
}
