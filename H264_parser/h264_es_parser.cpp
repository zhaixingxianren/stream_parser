#include "h264_es_parser.h"

int g_level = 0;

/*not check null pointer in local function */

/*
 * we here only parse 0x00 0x00 0x00 0x01 start code
 * as 0x00 0x00 0x01 is part of nalu data.....
 */
static int find_start_code(const unsigned char *start, int size)
{
    int  i = 0;
    const unsigned char *p = start;
    if(size< 7){
        Log(1,"error length");
        return -1;
    }

    while( i + 4 <= size ){
        if( (p[i] + p[i+1]) ==0U ){ //check fist 2 Zero
            if(p[i+2] == 0x00U && p[i+3] == 0x01U){
//                Log(1,"start code offset(0001) i=%d ",i);
                break;
            }        }
        i++;
    }

    if( i + 4 > size ){
        Log(1," return -1 now, i=%d",i);
        return -1;
    }

    return i;
}

int parse_h264_nul_type(const unsigned char* start, size_t size,size_t offset)
{
    const unsigned char * p =start;
    int nal_type = NAL_FF_IGNORE;
    if (!start || size < 7){
        Log(1,"null pointer or error length");
        return -1;
    }
    int ret = 1;
    unsigned int left = size;
    while(ret >= 0 && left >  4){
        ret = find_start_code(p,left);
        if ( ret < 0 ) { Log(1,"Not found !"); return -1;}

        p = p + ret + 4;
        left = size -( p-start);

        nal_type = p[0] & 0x1F ; //111 0, 0000
        switch (nal_type){
            case NALU_TYPE_SLICE    :
                Log(1,"-- OFFSET:%#lx, slice type",p-start-4 + offset);
                break;
            case NALU_TYPE_DPA      :
                Log(1,"-- OFFSET:%#lx, dpa type",p-start-4 + offset);
                break;
            case NALU_TYPE_DPB      :
                Log(1,"-- OFFSET:%#lx, dpb type",p-start-4 + offset);
                break;
            case NALU_TYPE_DPC      :
                Log(1,"-- OFFSET:%#lx, dpc type",p-start-4 + offset);
                break;
            case NALU_TYPE_IDR      :
                Log(1,"-- OFFSET:%#lx, IDR type",p-start-4 + offset);
                break;
            case NALU_TYPE_SEI      :
                Log(1,"-- OFFSET:%#lx, SEI type",p-start-4 + offset);
                break;
            case NALU_TYPE_SPS      :
                Log(1,"-- OFFSET:%#lx, SPS type",p-start-4 + offset);
                break;
            case NALU_TYPE_PPS      :
                Log(1,"-- OFFSET:%#lx, PPS type",p-start-4 + offset);
                break;
            case NALU_TYPE_AUD      :
                Log(1,"-- OFFSET:%#lx, AUD type",p-start-4 + offset);
                break;
            case NALU_TYPE_EOSEQ    :
                Log(1,"EOSEQ type");
                Log(1,"-- OFFSET:%#lx, EOSEQ type",p-start-4 + offset);
                break;
            case NALU_TYPE_EOSTREAM :
                Log(1,"-- OFFSET:%#lx, EOSSTREAM type",p-start-4 + offset);
                break;
            case NALU_TYPE_FILL     :
                Log(1,"-- OFFSET:%#lx, FILL type",p-start-4 + offset);
                break;
            case NAL_SPS_EXT        :
                Log(1,"-- OFFSET:%#lx, SPS_EXT type",p-start-4 + offset);
                break;
            case NAL_AUXILIARY_SLICE:
                Log(1,"-- OFFSET:%#lx, AUXILIARY_SLICE type",p-start-4 + offset);
                break;
            case NAL_FF_IGNORE      :
                Log(1,"-- OFFSET:%#lx, FF_IGNORE type",p-start-4 + offset);
                break;
            default:break;
        }

    }

    return ret;
}

//test
const static int UNIT_SIZE=4096;
const static int PADDING=3;
int main(int argc,char ** argv){

    if(argc != 2){
        Log(1,"error usage: bin + file_to_parse");
        return -1;
    }else Log(1,"file is %s ",argv[1]);

    char buf[ UNIT_SIZE ];
    size_t parse_size = UNIT_SIZE;
    std::string f_path{argv[1]};
    std::ifstream fin;

    fin.open(f_path.c_str(),std::ios::binary);
    fin.read(buf,parse_size);

    int read_times = 0 ;// count number;
    while( !fin.eof()){ //check EOF

        parse_h264_nul_type((unsigned char *)buf, parse_size, read_times*(UNIT_SIZE-PADDING));

        return 0;
        //copy last padding to head
        buf[0] = buf[parse_size-4];
        buf[1] = buf[parse_size-3];
        buf[2] = buf[parse_size-2];
        buf[3] = buf[parse_size-1];

        if(!fin.read(buf+PADDING,parse_size-PADDING)){
            Log(1,"here:%d ?? ",read_times);
        }

        read_times++;
    }

    Log(1,"exit now,if not print start code offset,then not found");
    return 0;
}

