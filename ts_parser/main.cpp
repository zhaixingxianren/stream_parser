/* author: lijinbo@hisense.com */
/* date  : 20170621            */

#include<tsparser.h>
#include<log.h>

static char opt[2]={0,0};
static char * file_path = NULL;

void parse_option(int argc, char ** argv);
void printusage();



int main(int argc, char ** argv)
{
	g_level = getLevel();
	if(argc < 2 ||  argc > 5 ){
		printusage();
		return 0;
	}

	parse_option(argc,argv);

	TS_Parser ts_parser(file_path);
    ts_parser.parsePAT();
    ts_parser.parsePMT();

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
	"\t-off hex\tprint 'hex' position or near-after positon's ts-pkg header info\n"
	"\t\t\tif have video,print I/B/P frame type ,and a/v's time.\n"
	"\t-pat\tprint pat ts-info\n"
	"\t-pmt\tprint pmt ts-info\n"
	"\t-media\tprint a/v info (codec,h/w)\n"
	);
}

void parse_option(int argc, char ** argv)
{
	if(argv == NULL){
		goto err_out;
		return;
	}

	switch (argc){
		case 2: //no option, -t as default
			opt[0] = 't';
			break;
		case 3: // 1 option
			opt[0] = argv[1][0];
			break;
		case 4: // 2 option
			opt[0] = argv[1][0];
			opt[1] = argv[2][0];
			break;
		default:
			goto err_out;
			break;
	}
	
	file_path = argv[argc-1];

	//for debug
	for(int i =0 ;i< argc;i++)
		Log(Info,"%s",argv[i]);

	return;
err_out:
	printusage();
}

