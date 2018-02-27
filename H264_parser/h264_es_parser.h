#include <iostream>
#include <fstream>
#include <string>
#include<stdio.h>
#include<sys/types.h>

extern int g_level;
#define Log(level,...)   \
	do {			 	 \
		if(level > g_level){		 \
			fprintf(stderr," [%s] ",__FUNCTION__); \
			fprintf(stderr,__VA_ARGS__); \
			fprintf(stderr,"\n");   \
		}							\
	}while(0)

#define log(lv,...)   \
	do {			 	 \
		if(lv > g_level){		 \
			fprintf(stderr,__VA_ARGS__); \
		}							\
	}while(0)

typedef enum {
    NALU_TYPE_SLICE    =0x1,
    NALU_TYPE_DPA      =0x2,
    NALU_TYPE_DPB      =0x3,
    NALU_TYPE_DPC      =0x4,
    NALU_TYPE_IDR      =0x5,
    NALU_TYPE_SEI      =0x6,
    NALU_TYPE_SPS      =0x7,
    NALU_TYPE_PPS      =0x8,
    NALU_TYPE_AUD      =0x9,
    NALU_TYPE_EOSEQ    =0xa,
    NALU_TYPE_EOSTREAM =0xb,
    NALU_TYPE_FILL     =0xc,
    NAL_SPS_EXT        =0xd,
    NAL_AUXILIARY_SLICE=0xe,
    NAL_FF_IGNORE      = 0xff0f001,
} NAL_TYPE;

typedef struct {
    unsigned char forbidden_zero_bit;
    unsigned char nal_ref_idc;
    unsigned char nal_unit_type;
} NAL_HEADER;

