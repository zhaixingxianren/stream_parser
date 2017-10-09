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

void init_curl(void);
void deinit_curl(void);

class EASY_HTTP
{
public:
    EASY_HTTP()
    {
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
    }

    HTTP_st downloader_url(const char * url);
    HTTP_st downloader_file(const char * file);
    HTTP_st downloader_useragent(const char *);
    HTTP_st downloader_start();

private:
    CURL *curl{NULL};
    CURLcode res;
    int fd{-1};


    EASY_HTTP(const EASY_HTTP&) = delete;
    EASY_HTTP & operator=(const EASY_HTTP&) = delete;
};
