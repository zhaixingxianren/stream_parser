#include <streamio.h>
#include <log.h>


namespace STREAMIO{
	
IOMethod::IOMethod(char *url):m_url(url)
{
	if(strncmp( url, "http://",7 ) == 0){
		m_scheme = eHttp;
		Log(Info,"scheme: eHttp, m_fd:%d (%s) \n",m_fd,m_url);
	}else if(strncmp( url, "file://",7 ) == 0){
		m_scheme = eFile;
		m_url = m_url + 6;
		m_fd = open(m_url,O_RDONLY);
		Log(Info,"scheme: eFile, m_fd:%d (%s)\n",m_fd,m_url);
	}else if(strncmp( url, "ftp://",6 ) == 0){
		Log(Info,"scheme: eHttp, m_fd:%d (%s)\n",m_fd,m_url);
		m_scheme = eFtp;
	}else if(strstr(url,"://") == NULL){ //local file
		m_scheme = eFile;
		m_url = m_url;
		m_fd = open(m_url,O_RDONLY);
		Log(Info,"scheme: eFile, m_fd:%d (%s)\n",m_fd,m_url);
	}else {
		m_scheme = eUnknown;
		Log(Info,"scheme: unknown (%s)\n",m_url);
	}

	if(m_fd == -1) {
		Log(Error,"open file error");
		exit(1);
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
	Log(Info,"IOMethod,m_fd:%d (%s) \n",m_fd,m_url);
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
	Log(Debug,"\n");
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
