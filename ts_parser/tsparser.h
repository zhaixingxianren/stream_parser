#include <vector>
#include <stdint.h>
#include <streamio.h>

/********************* PID         value
PAT         0x0000
CAT         0x0001
TSDT        0x0002
EIT,ST      0x0012
RST,ST      0x0013
TDT,TOT,ST  0x0014
**********************/

/* temporarily only suppport TS-PKG-SIZE = 188 */
const uint8_t TS_PKG_LENGTH = 188;

typedef struct TS_PAT_Program
{
    uint32_t program_number   :  16;     // program number.
    uint32_t program_map_PID  :  13;     // program map pid.
}TS_PAT_Program;

typedef struct
{
    uint8_t table_id                     : 8; //always eq 0x00 ，meaning PAT table.
    uint16_t section_syntax_indicator     : 1; //always eq 1.
    uint16_t zero                         : 1; //0
    uint16_t reserved_1                   : 2; // resolved
    uint16_t section_length              : 12; //表示从下一个字段开始到CRC32(含)之间有用的字节数
    uint16_t transport_stream_id          : 16; //stream id ID，区别于一个网络中其它多路复用的流
    uint8_t reserved_2                   : 2;  // resolved
    uint8_t version_number               : 5; //range: 0-31，PAT version.
    uint8_t current_next_indicator       : 1; //valid current or next.
    uint8_t section_number               : 8; //section number. PAT may transfered by multi-section，first section is 00，and increased 1 every section,max is 256.
    uint8_t last_section_number          : 8;  //last section number
 
    std::vector<TS_PAT_Program> program;
    uint16_t reserved_3                   : 3; // resolved
    uint16_t network_PID                  : 13; //NIT PID, 节目号为0时对应的PID为network_PID
    uint32_t CRC_32                       : 32;  //CRC32 checksum.
} PAT_st;

typedef struct {
} PMT_st;

/*
   TS struct, as bit-order difference, can not just copy stream[0x47,0x40,0x11,...] to this struct
 */
//#pragma pack (1)
typedef struct {
	uint8_t SYNCBYTE;   //byte[0]

	uint16_t transport_error_indicator:1;    //byte[1]
	uint16_t payload_unit_start_indicator:1; //byte[1]
	uint16_t transport_priority:1;           //byte[1]
        uint16_t PID:13;                         //byte[1] byte[2]


	uint8_t transport_scrambing_control:2;   //byte[3]
   	uint8_t adaptation_field_control:2;      //byte[3]
	uint8_t continuity_counter:4;            //byte[3]
	
	uint8_t adaptation_field_length;
	uint8_t discontinuity_indicator:1;
	uint8_t random_access_indicator:1;
	uint8_t elementary_stream_priority_indicator:1;
	uint8_t _5_flags:5;
	uint64_t PCR;
	uint64_t OPCR;
	uint8_t spice_countdown;
	uint8_t transport_private_data_length;
	uint8_t * ts_private_data;
	uint8_t adaptation_field_extension_length;
	uint8_t _3_flags;
} TS_Header_st;
//#pragma pack ()

typedef struct {
} PES_Header_st;

class TS_Parser{
	public:
		//open fd, prepare resource
		TS_Parser(char * fd); 

		//close fd,release resource
		~TS_Parser(); 

		void parsePAT();
		void parsePMT();

		//find the first offset of pid
		uint64_t getOffsetOfPid(uint16_t pid);

	private: //disabled;
		TS_Parser();

	private:
		void sync_offset();
		void set_ts_header(TS_Header_st & header, uint32_t offset);
		void set_pat(PAT_st & pat, uint32_t offset);

		const static uint8_t SYNCBYTE=0x47;
		STREAMIO::IOMethod * m_iomethod;
		uint32_t first_sync_offset{0}; //fist 0x47
		uint32_t PAT_pkgnum{0};
		//TS_Header_st ts_header{0};
		PAT_st pat_info;
		PMT_st pmt_info;
		uint8_t  local_buffs[TS_PKG_LENGTH * 200]; // 200 times length.
};
