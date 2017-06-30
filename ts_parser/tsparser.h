#include <stdint.h>

/* temporarily only suppport TS-PKG-SIZE = 188 */

typedef struct {
} PAT_st;

typedef struct {
} PMT_st;

/*
   TS struct
 */
typedef struct {
	//const static uint8_t SYNCBYTE=0x47;
	uint16_t transport_error_indicator:1;
	uint16_t payload_unit_start_indicator:1;
	uint16_t transport_priority:1;
	uint16_t PID:13;
	uint8_t transport_scrambing_control:2;
	uint8_t adaptation_field_control:2;
	uint8_t continuity_counter:4;
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

typedef struct {
} PES_Header_st;

class TS_Parser{
	public:
		//open fd, prepare resource
		TS_Parser(char * fd); 

		//close fd,release resource
		~TS_Parser(); 

		void sync_offset();
		void PAT_pkg_offset(int fd);

	private: //disabled;
		TS_Parser();

	private:
		const static uint8_t SYNCBYTE=0x47;
		char * ts_file{nullptr};
		uint32_t ts_fd{0};
		uint64_t first_sync_offset{0}; //fist 0x47
		uint32_t PAT_pkgnum{0};
		TS_Header_st ts_header{0};
		PAT_st pat_info;
		PMT_st pmt_info;
};
