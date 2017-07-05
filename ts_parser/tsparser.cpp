#include <tsparser.h>

#include <iostream>

#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <log.h>

LogLevel g_level = Info;

void print_pat(PAT_st & pat);
void printf_ts_header(const TS_Header_st & st);


TS_Parser::TS_Parser(char * file)
{
    Log(Debug,"calling");
    m_iomethod = new STREAMIO::IOMethod(file);
	sync_offset();
}

TS_Parser::~TS_Parser()
{
    Log(Debug,"calling");
    delete m_iomethod;
}


void TS_Parser::parsePAT()
{
   	TS_Header_st header = {0};
    uint32_t offset=  getOffsetOfPid(0);

	set_ts_header(header,offset);

	if(header.payload_unit_start_indicator){
		switch(header.adaptation_field_control){
			case 0: //0x00 resolved
				break;
			case 1: //0x01, no adaptation, 184 payload
				set_pat(pat_info,offset+4);
				break;
			case 2: //0x10, no payload, 183 adaptation 
				break;
			case 3: //0x11, paylaod after 0~182B adaptation
				break;
		}

	}
}

/*
 * header, ts struct
 * offset, the ts header's offset
 */
void TS_Parser::set_pat(PAT_st & pat, uint32_t offset)
{
	uint8_t * bytes8 = local_buffs + offset;
	Log(Info, "pointer feild:%#x",bytes8[0]);
	bytes8 = bytes8[0] == 0 ? bytes8+1 : bytes8+bytes8[0] ;

//	for(int i =0;i < 10; i++) 
//		fprintf(stderr,"%x ",bytes8[i]);

	pat.table_id = bytes8[0];
	pat.section_syntax_indicator = bytes8[1] >> 7;
	pat.section_length = (((uint16_t)bytes8[1] & 0x0f) << 8) + bytes8[2]; //count from stream-id to crc
	pat.transport_stream_id = ( (uint16_t)bytes8[3] << 8) + bytes8[4];
	pat.version_number = (bytes8[5] & 0x3f) > 1;
	pat.current_next_indicator = bytes8[5] & 0x1;

	//get program
	int16_t section_length = pat.section_length > 5 ? pat.section_length -5 : 0 ;

	Log(Info,"section length:%d",section_length);
	while((section_length -= 4) > 0){
		TS_PAT_Program pgm{0};
		pgm.program_number = ((uint16_t)bytes8[8] << 8) + bytes8[9];
		if(pgm.program_number == 0x00 ){
			pat.network_PID     = ( ( (uint16_t)bytes8[10] & 0x1f) << 8) + bytes8[11];
		}else{
			///TODO: program_number=0x01 is pmt ?
			pgm.program_map_PID = ( ( (uint16_t)bytes8[10] & 0x1f) << 8) + bytes8[11];
			fprintf(stderr,"left :%d\n",section_length);
			pat.program.push_back(pgm);
		}
	}
	print_pat(pat);
}

void TS_Parser::parsePMT()
{
   	TS_Header_st header = {0};
    uint32_t offset=  getOffsetOfPid(pat_info.program[0].program_map_PID);

	set_ts_header(header,offset);

	if(header.payload_unit_start_indicator){
		switch(header.adaptation_field_control){
			case 0: //0x00 resolved
				break;
			case 1: //0x01, no adaptation, 184 payload
				//set_pmt(pat_info,offset+4);
				break;
			case 2: //0x10, no payload, 183 adaptation 
				break;
			case 3: //0x11, paylaod after 0~182B adaptation
				break;
		}

	}
}

void TS_Parser::set_pmt(PMT_st & pmt, uint32_t offset)
{
	uint8_t * bytes8 = local_buffs + offset;
	Log(Info, "pointer feild:%#x",bytes8[0]);
	bytes8 = bytes8[0] == 0 ? bytes8+1 : bytes8+bytes8[0] ;
}
/*
 * header, ts struct
 * offset, the ts header's offset
 */
void TS_Parser::set_ts_header(TS_Header_st & header, uint32_t offset)
{
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

void print_pat(PAT_st & pat)
{
	Log(Info,"\ttable_id = %#x, \n"
			 "\tsection_syntax_indicator = %#x ,\n"
			 "\tsection_length = %#x , \n"
			 "\ttransport_stream_id = %#x, \n"
			 "\tpat.version_number = %#x,\n"
			 "\tpat.current_next_indicator=%#x, \n"
			 ,
	pat.table_id,
	pat.section_syntax_indicator,
	pat.section_length,
	pat.transport_stream_id,
	pat.version_number,
	pat.current_next_indicator
	);
	
	std::vector<TS_PAT_Program>::iterator it = pat.program.begin();
	for(;it != pat.program.end(); it++){
		Log(Info,"\tprogram_id:%#x , program map pid:%#x \n"
				,it->program_number
				,it->program_map_PID);
	}

}
