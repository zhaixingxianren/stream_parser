/* author: lijinbo@hisense.com */
/* date  : 20170621            */

#include<tsparser.h>
#include<log.h>

enum OptType{
	ePKGNumInfo, //print pkgnum's info
	eOffsetInfo, //print offset's pkg info
	ePSIInfo,    //print psi info
	eMediaInfo  //print media-info
};

struct option{
	OptType type;
	int pkg_num;
	int offset;
};

static struct option opts;
static char * file_path = NULL;

void parse_option(int argc, char ** argv);
void printusage();



int main(int argc, char ** argv)
{
	g_level = getLevel();
	if(argc < 2 ){
		printusage();
		return 0;
	}

	parse_option(argc,argv);

	TS_Parser ts_parser(file_path);
    ts_parser.parsePAT();
    ts_parser.parsePMT();

	switch (opts.type)
	{
		case ePKGNumInfo: // -l number
			break;
		case eOffsetInfo: //-p offset
			break;
		case ePSIInfo: // -i 
			ts_parser.print_pat();
			ts_parser.print_pmt();
			break;
		case eMediaInfo: // -m mediainfo
			break;
	}

	return 0;
}

void printusage()
{
	Log(Info,"usage: \n"
	" [tools] [option] [files]\n"
	" option:\n"
	"\t-l [m,n]\tlist ts-pkgs header info , from m to n. default list first 5 ts header info\n"
	"\t\t\tif n not set, will only print m pkgs info.\n"
	"\t\t\tif have video,print I/B/P frame type ,and a/v's time.\n"
	"\t-p hex\tprint 'hex' position or near-after positon's ts-pkg header info\n"
	"\t\t\tif have video,print I/B/P frame type ,and a/v's time.\n"
	"\t-i\tprint pat/pmt ts-info\n"
	"\t-m\tprint a/v info (codec,h/w)\n"
	"\n\nexport PARSER_LEVEL=Debug/Info/Error\n"
	);
}

void parse_option(int argc, char ** argv)
{
	int opt = -1;
	if(argv == NULL){
		goto err_out;
		return;
	}

	while( (opt = getopt(argc,argv,"l:p:im" )) != -1 ) {
		switch (opt){
			case 'l': //list pkg-num's info
				opts.type = ePKGNumInfo;  
				opts.pkg_num = atoi(optarg) ;
				Log(Debug,"ePKGNumInfo option,%d",opts.pkg_num);
				break;
			case 'p': // 1 option
				opts.type = eOffsetInfo; 
				opts.offset = atoi(optarg);
				Log(Debug,"eOffsetInf option,%d",opts.offset);
				break;
			case 'i': // 2 option
				opts.type = ePSIInfo; // psi info
				Log(Debug,"ePSIInfo option");
				break;
			case 'm': // 2 option
				opts.type = eMediaInfo; //media info
				Log(Debug,"eMediaInfo option");
				break;
			default:
				opts.type = eMediaInfo; //no option, -t as default
				Log(Debug,"defalt option");
				break;
		}
	}

	file_path = argv[argc-1];

	//for debug
	for(int i =0 ;i< argc;i++)
		Log(Debug,"%d --> %s",i,argv[i]);

	return;
err_out:
	printusage();
}

