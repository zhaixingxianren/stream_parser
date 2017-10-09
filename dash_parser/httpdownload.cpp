#include <httpdownload.h>

int g_level = 0;

static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    int ret = 0;
    int * fd = (int *)userp;
    Log(1,"fd:%d ,size:%ld ",*fd,size * nmemb);
    ret = write( *fd, contents, size * nmemb );
    return ret;
}

HTTP_st EASY_HTTP::url(const char * url)
{
    if(curl && url ){
        curl_easy_setopt(curl, CURLOPT_URL,url );
        curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,write_callback );
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,(void *)&fd );
        return HTTP_OK;
    }
    Log(1,"error here");
    return HTTP_NG;
}

HTTP_st EASY_HTTP::local_file(const char * file)
{
    fd = open (file,O_RDWR | O_CREAT,0777);
    return HTTP_OK;
}

HTTP_st EASY_HTTP::useragent(const char * ua)
{
    if(curl && ua)
      curl_easy_setopt(curl, CURLOPT_USERAGENT, ua);

    return HTTP_OK;
}

HTTP_st EASY_HTTP::start_download()
{
    if(curl){
        res = curl_easy_perform(curl);
        return HTTP_OK;
    }
    Log(1,"error here");
    return HTTP_NG;
}
