/*
   for read/write data from url
 */

namespace STREAMIO{
	enum SCHEME {
		eFile, //file://
		eHttp, // http://
		eFtp,  //rtp://
	};
	
	class IOMethod{
		public:
		IOMethod(char * url);
		IOMethod(char * url,SCHEME scheme);
		~IOMethod();

		size_t io_read(char *buf, size_t size);

		private:
		char * m_url;
		int m_fd;
		SCHEME m_scheme;

		IOMethod(){};
	};
}
