#include <vector>
#include <stdint.h>
#include <streamio.h>

/********************* PID         value
PAT         0x0000
PMT         0x0001 
TSDT        0x0002
EIT,ST      0x0012
RST,ST      0x0013
TDT,TOT,ST  0x0014


[Table-id] :
Decimal	Hexadecimal	Description
0		0x00		Program Association section contains a directory listing of all Program Map Tables
1		0x01		Conditional Access section contains a directory listing of all EMM streams
2		0x02		Program Map section contains a directory listing of all elementary streams.
3		0x03		Transport Stream Description section.
4		0x04		ISO/IEC 14496 scene description section.
5		0x05		ISO/IEC 14496 object description section.
6		0x06		Metadata section.
7		0x07		ISO/IEC 13818-11 IPMP control information (DRM).
8		57			0x08 - 0x39	Reserved.
58		0x3A		ISO/IEC 13818-6 DSM CC multiprotocol encapsulated.
59		0x3B		ISO/IEC 13818-6 DSM CC U-N messages.
60		0x3C		ISO/IEC 13818-6 DSM CC Download Data Messages.
61		0x3D		ISO/IEC 13818-6 DSM CC stream descriptor list.
62		0x3E		ISO/IEC 13818-6 DSM CC privately defined (DVB MAC addressed datagram).
63		0x3F		ISO/IEC 13818-6 DSM CC addressable.
64 - 127	0x40 - 0x7F	Used by DVB.
128 - 143	0x80 - 0x8F	DVB-CSA and DigiCipher II/ATSC CA message sections used in EMM and ECM streams.
144 - 191	0x90 - 0xBF	May be assigned as needed to other data tables.
192 - 254	0xC0 - 0xFE	Used by DigiCipher II/ATSC/SCTE.
255	0xFF	Forbidden. As is used for null padding.

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


typedef struct TS_PMT_Stream  
{  
	uint8_t  stream_type                       : 8; //指示特定PID的节目元素包的类型。该处PID由elementary PID指定  
	uint32_t elementary_PID                    : 13; //该域指示TS包的PID值。这些TS包含有相关的节目元素  
	uint32_t ES_info_length                    : 12; //前两位bit为00。该域指示跟随其后的描述相关节目元素的byte数  
	uint32_t descriptor;  
}TS_PMT_Stream;

typedef struct {
	uint8_t table_id                        : 8; //always 0x02, mean PMT table 
	uint16_t section_syntax_indicator        : 1; //always = 0x01  
    uint16_t zero                            : 1; //0x01  
    uint16_t reserved_1                      : 2; //0x03  
    uint16_t section_length                  : 12;//首先两位bit置为00，它指示段的byte数，由段长度域开始，包含CRC。  
    uint16_t program_number                    : 16;// 指出该节目对应于可应用的Program map PID  
    uint8_t reserved_2                        : 2; //0x03  
    uint8_t version_number                    : 5; //指出TS流中Program map section的版本号  
    uint8_t current_next_indicator            : 1; //当该位置1时，当前传送的Program map section可用；  
                                                     //当该位置0时，指示当前传送的Program map section不可用，下一个TS流的Program map section有效。  
    uint8_t section_number                    : 8; //固定为0x00  
    uint8_t last_section_number            : 8; //固定为0x00  
    uint16_t reserved_3                        : 3; //0x07  
    uint16_t PCR_PID                        : 13; //指明TS包的PID值，该TS包含有PCR域，  
													//该PCR值对应于由节目号指定的对应节目。  
													//如果对于私有数据流的节目定义与PCR无关，这个域的值将为0x1FFF。  
    uint16_t reserved_4                        : 4; //预留为0x0F  
    uint16_t program_info_length            : 12; //前两位bit为00。该域指出跟随其后对节目信息的描述的byte数。  
      
    std::vector<TS_PMT_Stream> PMT_Stream;  //每个元素包含8位, 指示特定PID的节目元素包的类型。该处PID由elementary PID指定  
    uint8_t reserved_5                        : 3; //0x07  
    uint8_t reserved_6                        : 4; //0x0F  
    uint32_t CRC_32                            : 32; 
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
		void print_pmt();
		void print_pat();
        void filter_pid(unsigned int pid);

		//find the first offset of pid
		uint64_t getOffsetOfPid(uint16_t pid);

	private: //disabled;
		TS_Parser();

	private:
		void sync_offset();
		void set_ts_header(TS_Header_st & header, uint32_t offset);
		void set_pat(PAT_st & pat, uint32_t offset);
		void set_pmt(PMT_st & pmt, uint32_t offset);

		const static uint8_t SYNCBYTE=0x47;
		STREAMIO::IOMethod * m_iomethod;
		uint32_t first_sync_offset{0}; //fist 0x47
		uint32_t PAT_pkgnum{0};
		//TS_Header_st ts_header{0};
		PAT_st pat_info;
		PMT_st pmt_info;
		uint8_t  local_buffs[TS_PKG_LENGTH * 200]; // 200 times length.
};
