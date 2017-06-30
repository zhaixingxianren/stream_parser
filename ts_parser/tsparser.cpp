#include <tsparser.h>

#include <iostream>

#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using std::cout;
using std::endl;

TS_Parser::TS_Parser(char * file)
{
	if(file)  ts_fd = open(file,O_RDONLY);

    cout<<__FUNCTION__<<"file:"<<file<<",fd="<<ts_fd<<endl;	
}

TS_Parser::~TS_Parser()
{
	if(ts_fd)  close(ts_fd);
}


void TS_Parser::sync_offset()
{
	uint8_t buf8[188*3];
	uint8_t check_time = 0;
	long offset = lseek(ts_fd, 0, SEEK_CUR);
	while (check_time++ <2 && read(ts_fd,&buf8,188*3) > 0){
		uint8_t i = 0;
		for(; i< 188; i++){
			if(buf8[i] == SYNCBYTE && buf8[i+1*188] == SYNCBYTE && buf8[i+2*188] == SYNCBYTE){
				first_sync_offset = offset + i + 188 * (check_time-1);
				cout<<"found ts sync-bye,offset:"<<first_sync_offset<<endl;
				lseek(ts_fd, first_sync_offset,SEEK_SET);
				break;
			}
		}
		if (i<188) break;
	}

	if(check_time > 2) {
		cout<<"ts sync-byte not found !"<<endl;
		lseek(ts_fd, first_sync_offset,SEEK_SET);
	}
}
