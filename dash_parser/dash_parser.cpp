#include<iostream>
#include <httpdownload.h>

void print_usage()
{
    printf("dash_parser mpd_url [info]");
}

int main(int argc,char **argv)
{
    if(argc <=1 )
        print_usage();

    Log(1,"download url:%s",argv[1]);

    char * ua = "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/61.0.3163.100 Safari/537.36";

    init_curl();
    {
        EASY_HTTP easyHttp;
        easyHttp.downloader_file("./dash.mpd");
        easyHttp.downloader_url(argv[1]);
        easyHttp.downloader_useragent(ua);
        easyHttp.downloader_start();
    }
    deinit_curl();
}
