#include <stdio.h>
#include <curl/curl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

extern int g_level;
#define Log(level,...)   \
    do {                 \
        if(level > g_level){         \
            fprintf(stderr," [%s] ",__FUNCTION__); \
            fprintf(stderr,__VA_ARGS__); \
            fprintf(stderr,"\n");   \
            fflush(stderr)  ;        \
        }                           \
    }while(0)

typedef enum{
    HTTP_OK,
    HTTP_RETRY,
    HTTP_NG,
}HTTP_st;


class EASY_HTTP
{
public:
    EASY_HTTP()
    {
        curl_global_init( CURL_GLOBAL_ALL );
        curl = curl_easy_init();
        if(!curl){
            Log(1,"init error");
        }
    }

    ~EASY_HTTP()
    {
        if(curl){
           curl_easy_cleanup(curl);
        }
        curl_global_cleanup();
    }

    HTTP_st url(const char * url);
    HTTP_st local_file(const char * file);
    HTTP_st useragent(const char *);
    HTTP_st start_download();

private:
    CURL *curl{NULL};
    CURLcode res;
    int fd{-1};

    EASY_HTTP(const EASY_HTTP&) = delete;
    EASY_HTTP & operator=(const EASY_HTTP&) = delete;
};



