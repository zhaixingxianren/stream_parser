/*************************************************************************************
Elementary stream types[edit]
Each elementary stream in a transport stream is identified by an 8-bit elementary stream type assignment.

Assignments in use
************************************************************************************/

type struct {
	char type_num;
	char * type_string;
}StreamType;

/* reserved and private using is ignored */

/*Hexadecimal<-->Description */
StreamType streamtypeMap[] ={
	{0x01,	"ISO/IEC 11172-2 (MPEG-1 video)" },
	{0x02,{"ITU-T Rec. H.262 and ISO/IEC 13818-2 (MPEG-2 higher rate interlaced video)"},
	{0x03,{"ISO/IEC 11172-3 (MPEG-1 audio)"},
	{0x04,{"ISO/IEC 13818-3 (MPEG-2 halved sample rate audio)"},
	{0x05,{"ITU-T Rec. H.222 and ISO/IEC 13818-1 (MPEG-2 tabled data)"},
	{0x06,{"ITU-T Rec. H.222 and ISO/IEC 13818-1 (MPEG-2 packetized data)"},
	{0x07,{"ISO/IEC 13522 (MHEG)"},
	{0x08,{"ITU-T Rec. H.222 and ISO/IEC 13818-1 DSM CC"},
	{0x09,{"ITU-T Rec. H.222 and ISO/IEC 13818-1/11172-1 auxiliary data"},
	{0x0A,{"ISO/IEC 13818-6 DSM CC multiprotocol encapsulation"},
	{0x0B,{"ISO/IEC 13818-6 DSM CC U-N messages"},
	{0x0C,{"ISO/IEC 13818-6 DSM CC stream descriptors"},
	{0x0D,{"ISO/IEC 13818-6 DSM CC tabled data"},
	{0x0E,{"ISO/IEC 13818-1 auxiliary data"},
	{0x0F,{"ISO/IEC 13818-7 ADTS AAC (MPEG-2 lower bit-rate audio)"},
	{0x10,{"ISO/IEC 14496-2 (MPEG-4 H.263 based video)"},
	{0x11,{"ISO/IEC 14496-3 (MPEG-4 LOAS multi-format framed audio)"},
	{0x12,{"ISO/IEC 14496-1 (MPEG-4 FlexMux)"},
	{0x13,{"ISO/IEC 14496-1 (MPEG-4 FlexMux)"},
	{0x14,{"ISO/IEC 13818-6 DSM CC synchronized download protocol"},
	{0x15,{"Packetized metadata"},
	{0x16,{"Sectioned metadata"},
	{0x17,{"ISO/IEC 13818-6 DSM CC Data Carousel metadata"},
	{0x18,{"ISO/IEC 13818-6 DSM CC Object Carousel metadata"},
	{0x19,{"ISO/IEC 13818-6 Synchronized Download Protocol metadata"},
	{0x1A,{"ISO/IEC 13818-11 IPMP"},
	{0x1B,{"ITU-T Rec. H.264 and ISO/IEC 14496-10 (lower bit-rate video)"},
	{0x24,{"ITU-T Rec. H.265 and ISO/IEC 23008-2 (Ultra HD video)"},
	{0x42,{"Chinese Video Standard"},
	{0x80,{"ITU-T Rec. H.262 and ISO/IEC 13818-2 for DigiCipher IIor PCM audio for Blu-ray"},
	{0x81,{"Dolby Digital up to six channel audio for ATSC and Blu-ray"},
	{0x82,{"SCTE subtitleor DTS 6 channel audio for Blu-ray"},
	{0x83,{"Dolby TrueHD lossless audio for Blu-ray"},
	{0x84,{"Dolby Digital Plus up to 16 channel audio for Blu-ray"},
	{0x85,{"DTS 8 channel audio for Blu-ray"},
	{0x86,{"SCTE-35[5] digital program insertion cue messageDTS 8 channel lossless audio for Blu-ray"},
	{0x87,{"Dolby Digital Plus up to 16 channel audio for ATSC"},
	{0x90,{"Blu-ray Presentation Graphic Stream (subtitling)"},
	{0x95,{"ATSC DSM CC Network Resources table"},
	{0xC0,{"DigiCipher II text"},
	{0xC1,{"Dolby Digital up to six channel audio with AES-128-CBC data encryption"},
	{0xC2,{"ATSC DSM CC synchronous data or Dolby Digital Plus up to 16 channel audio with AES-128-CBC data encryption"},
	{0xCF,{"ISO/IEC 13818-7 ADTS AAC with AES-128-CBC frame encryption"},
	{0xD1,{"BBC Dirac (Ultra HD video)"},
	{0xDB,{"ITU-T Rec. H.264 and ISO/IEC 14496-10 with AES-128-CBC slice encryption"},
	{0xEA,{"Microsoft Windows Media Video 9 (lower bit-rate video)"},
};
