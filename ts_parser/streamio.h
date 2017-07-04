/*
   for read/write data from url
 */
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

namespace STREAMIO{
	enum SCHEME {
		eFile, //file://
		eHttp, // http://
		eFtp,  //ftp://
		eUnknown,
	};
	
	class IOMethod{
		public:
		IOMethod(char * url);
		IOMethod(char * url,SCHEME scheme);
		~IOMethod();

		size_t io_read(uint8_t *buf, size_t size);

		private:
		char * m_url;
		int m_fd;
		SCHEME m_scheme;

		IOMethod(){};
	};
}
