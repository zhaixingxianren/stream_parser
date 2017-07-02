
#include <streamio.h>
#include <stdio.h>

#define Log(level,...) 

namespace STREAMIO{
	
IOMethod::IOMethod(char *url):m_url(url)
{
	if(strncmp( url, "http://",7 ) == 0){
		m_scheme = eHttp;
		fprintf(stderr,"scheme: eHttp, m_fd:%d (%s)",m_fd,m_url);
	}else if(strncmp( url, "file://",7 ) == 0){
		m_scheme = eFile;
		m_url = m_url + 6;
		m_fd = open(m_url,O_RDONLY);
		fprintf(stderr,"scheme: eFile, m_fd:%d (%s)",m_fd,m_url);
	}else if(strncmp( url, "ftp://",6 ) == 0){
		fprintf(stderr,"scheme: eHttp, m_fd:%d (%s)",m_fd,m_url);
		m_scheme = eFtp;
	}else{
		m_scheme = eUnknown;
		fprintf(stderr,"scheme: eUnknown,  (%s)",m_url);
	}

}

IOMethod::IOMethod(char *url,SCHEME scheme):m_url(url),m_scheme(scheme)
{
	switch (m_scheme)
	{
		case eFile:
		    m_fd = open(m_url,O_RDONLY);	
			break;
		case eHttp:
		case eFtp:
		default:
			break;
	}
	Log(Info,"IOMethod,m_fd:%d (%s) ",m_fd,m_url);
}

IOMethod::	~IOMethod()
{
	switch (m_scheme)
	{
		case eFile:
			if(m_fd) close(m_fd);
			break;
		case eHttp:
		case eFtp:
		default:
			break;
	}
	Log(Debug,"~IOMethod");
}

size_t IOMethod::io_read(uint8_t *buf, size_t size)
{
	switch (m_scheme)
	{
		case eFile:
			if(m_fd) {
				return read(m_fd,buf,size);
			}
			break;
		case eHttp:
		case eFtp:
		default:
			break;
	}
	return -1;
}

}//name space
