#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <streamio.h>

#define Log(level,...)

namespace STREAMIO{
	
IOMethod::IOMethod(char *url):m_url(url)
{
}

IOMethod::IOMethod(char *url,SCHEME scheme):m_url(url),m_scheme(scheme)
{
	switch (scheme)
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
	switch (scheme)
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

size_t IOMethod::io_read(char *buf, size_t size)
{
	switch (scheme)
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
}

