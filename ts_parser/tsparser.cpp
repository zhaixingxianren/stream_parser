#include <tsparser.h>

#include <iostream>

#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <log.h>

LogLevel g_level = Info;

TS_Parser::TS_Parser(char * file)
{
    m_iomethod = new STREAMIO::IOMethod(file);
    Log(Debug,"calling");
}

TS_Parser::~TS_Parser()
{
    Log(Debug,"calling");
    delete m_iomethod;
}

void printf_ts_header(const TS_Header_st & st)
{
	Log(Info,"####sync byte = %#x \n"
			"\ttransport_error_indicator\t = %#x \n"
			"\tpayload_unit_start_indicator\t = %#x \n"
			"\tpid\t = %#x \n"
			"\tscrambing _control\t = %#x \n"
			"\tadaptation_field_contorl\t = %#x \n"
			"\tcontinuity_counter\t = %#x \n",
			st.SYNCBYTE ,
			st.transport_error_indicator,
			st.payload_unit_start_indicator ,
			st.PID ,
			st.transport_scrambing_control ,
			st.adaptation_field_control ,st.continuity_counter
	);
}
void TS_Parser::parsePAT()
{
   	TS_Header_st header = {0};
   	uint32_t offset=  getOffsetOfPid(0);
	uint8_t * bytes8 = local_buffs + offset;
	header.SYNCBYTE = bytes8[0];
	header.transport_error_indicator    = bytes8[1] >> 7;
	header.payload_unit_start_indicator = bytes8[1] >> 6 & 0x01 ;
	header.PID    = ( ( (uint16_t)bytes8[1] & 0x1f) << 8) + bytes8[2];
    header.transport_scrambing_control =  bytes8[3] >> 6;
	header.adaptation_field_control = (bytes8[3] & 0x3f) >> 4 ;
	header.continuity_counter = (bytes8[3] & 0x0f)  ;

	printf_ts_header(header);
}

//get the offset of buffer.  from header of buffer.
uint64_t TS_Parser::getOffsetOfPid(uint16_t pid)
{
	for(uint16_t i = 0; i < 200; i ++){
		TS_Header_st header = {0};

		uint8_t * bytes8 = local_buffs + first_sync_offset + i * TS_PKG_LENGTH;

		header.SYNCBYTE = bytes8[0];
		header.transport_error_indicator    = bytes8[1] >> 7;
		header.payload_unit_start_indicator = bytes8[1] >> 6 & 0x01 ;
		header.PID                          = ( ( (uint16_t)bytes8[1] & 0x1f) << 8) + bytes8[2];
		Log(Debug,"sync:%#x,err-indicator:%d,start-indicator:%d,pid:%#x\n",header.SYNCBYTE,header.transport_error_indicator,
				header.payload_unit_start_indicator,header.PID);
		if(header.PID == pid){
			Log(Info,"found pid: i=%d !\n",i);
			return (first_sync_offset + i * TS_PKG_LENGTH);
		}
	}

	return 0;
}

void TS_Parser::sync_offset()
{
	uint32_t i=0;
        if( m_iomethod->io_read( local_buffs , sizeof(local_buffs)) <= 0 ) return;

	do{
	     if(local_buffs[i] == 0x47 &&  local_buffs[i+TS_PKG_LENGTH]== 0x47 &&  local_buffs[i+2*TS_PKG_LENGTH] == 0x47){
		first_sync_offset = i;
		return;
	      }
	} while(i++ && i < TS_PKG_LENGTH);

	Log(Error,"ts sync-byte not found !");
}
