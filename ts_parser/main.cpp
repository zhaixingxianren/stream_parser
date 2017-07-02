/* author: lijinbo@hisense.com */
/* date  : 20170621            */
#include<iostream>
#include<tsparser.h>

using std::cout;
using std::endl;

static char opt[2]={0,0};
static char * file_path = NULL;

void parse_option(int argc, char ** argv);

void printusage()
{
fprintf(stderr,"usage: \n"
" [tools] [option] [files]\n"
" option:"
"	 -l [m,n]		:    list ts-pkgs header info , from m to n. default list first 5 ts header info\n"
"						 if n not set, will only print m pkgs info.\n"
"						 if have video,print I/B/P frame type ,and a/v's time.\n"
"	 -off hex       :    print 'hex' position or near-after positon's ts-pkg header info\n"
"						 if have video,print I/B/P frame type ,and a/v's time.\n"
"	 -pat           :    print pat ts-info\n"
"	 -pmt           :    print pmt ts-info\n"
"	 -media         :    print a/v info (codec,h/w)\n"
);
}


int main(int argc, char ** argv)
{
	if(argc < 2 ||  argc > 5 ){
		printusage();
		return 0;
	}

	parse_option(argc,argv);
	TS_Parser ts_parser(file_path);
	ts_parser.sync_offset();
        ts_parser.parsePAT();
	return 0;
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

	cout<<"opt0:"<<opt[0]<<",opt1:"<<opt[1]<<endl;
	cout<<"file:"<<file_path<<endl;

	//for debug
	for(int i =0 ;i< argc;i++)
		cout<<argv[i]<<endl;

	return;
err_out:
	printusage();
}

