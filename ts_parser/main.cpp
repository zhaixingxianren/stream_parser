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
	cout<<"Usage: ts-parse-tools [Options]...  [TS-files]..."<<endl;
	cout<<"[Options]: "<<endl;
	cout<<"  -t  only parse ts header [default]"<<endl;
	cout<<"  -p  parse ts & pes header "<<endl;
	cout<<"  -s  parse specified stream pid "<<endl;
}


int main(int argc, char ** argv)
{
	if(argc < 2 ||  argc > 4 )
		printusage();

	parse_option(argc,argv);
	TS_Parser ts_parser(file_path);
	ts_parser.sync_offset();
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
	for(int i ;i< argc;i++)
		cout<<argv[i]<<endl;

	return;
err_out:
	printusage();
}

