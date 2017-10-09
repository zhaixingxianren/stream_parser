#include <httpdownload.h>

int g_level = 0;

/* only called once */
void init_curl(void)
{
    curl_global_init( CURL_GLOBAL_ALL );
}
/* only called once */
void deinit_curl(void)
{
    curl_global_cleanup();
}

static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    int ret = 0;
    int * fd = (int *)userp;
    Log(1,"fd:%d ,size:%ld ",*fd,size * nmemb);
    ret = write( *fd, contents, size * nmemb );
    return ret;
}

HTTP_st EASY_HTTP::downloader_url(const char * url)
{
    Log(1,"%s ",url);
    if(curl && url ){
        curl_easy_setopt(curl, CURLOPT_URL,url );
        curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,write_callback );
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,(void *)&fd );
        return HTTP_OK;
    }
    Log(1,"error here");
    return HTTP_NG;
}

HTTP_st EASY_HTTP::downloader_file(const char * file)
{
    fd = open (file,O_RDWR | O_CREAT,0777);
    Log(1,"fd:%d",fd);
    return HTTP_OK;
}

HTTP_st EASY_HTTP::downloader_useragent(const char * ua)
{
    if(curl && ua)
      curl_easy_setopt(curl, CURLOPT_USERAGENT, ua);

    return HTTP_OK;
}

HTTP_st EASY_HTTP::downloader_start()
{
    if(curl){
        res = curl_easy_perform(curl);
        return HTTP_OK;
    }
    Log(1,"error here");
    return HTTP_NG;
}
