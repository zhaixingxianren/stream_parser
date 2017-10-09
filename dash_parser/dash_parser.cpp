#include<iostream>
#include <httpdownload.h>

void print_usage()
{
    printf("dash_parser mpd_url [info]");
}

static const char * ua = "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/61.0.3163.100 Safari/537.36";

int main(int argc,char **argv)
{
    if(argc <=1 )
        print_usage();

    Log(1,"download url:%s",argv[1]);
 
    EASY_HTTP easyHttp;
    easyHttp.local_file("./dash.mpd");
    easyHttp.url(argv[1]);
    easyHttp.useragent(ua);
    easyHttp.start_download();

}
