#include<iostream>
#include <httpdownload.h>
#include <xml_wrapper.h>

void print_usage()
{
    printf("dash_parser mpd_url [info]");
}

static const char* LOCALFILE = "./dash.mpd";
static const char * ua = "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/61.0.3163.100 Safari/537.36";

int main(int argc,char **argv)
{
    if(argc <=1 ){
        print_usage();
        exit(1);
    }

    Log(1,"download url:%s",argv[1]);
 
    EASY_HTTP easyHttp;
    easyHttp.local_file(LOCALFILE);
    easyHttp.url(argv[1]);
    easyHttp.useragent(ua);
    easyHttp.start_download();

    XMLWrapper* xmlparse = new XMLWrapper(LOCALFILE);
    xmlNodePtr node = xmlparse->findNodeByName(xmlparse->getRootElement(),(xmlChar*)"MPD");
    if(node == NULL) { Log(1,"not found MPD"); return -1;}

    xmlChar * duration = xmlparse->getAttrValFromNode(node,(xmlChar*)"mediaPresentationDuration");
    if(node == NULL) { Log(1,"not found MPD"); return -1;}
    else Log(1,"mpd duration:%s",duration);
}
