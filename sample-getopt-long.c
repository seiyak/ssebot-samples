#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const char *nm;

void pusage(FILE *fp, int exd)
{
	FILE *mn = popen("man ls","r");
	if(mn == NULL)
		return;
	char buf[4096];
	size_t sz,i;
	while((sz = fread(buf,1,4096,mn)) > 0){
		for(i = 0; i < sz;i++)
			printf("%c",buf[i]);
	}
	pclose(mn);
	exit(exd);
}

int main(int argc,char *argv[])
{
	int nopt,v = -1,f = -1,g = -1;
	const char *sopts="hvfg:";
	const struct option lopts[] = {
		{"help",0,NULL,'h'},
		{"verbose",0,NULL,'v'},
		{"file",0,NULL,'f'},
		{"ggg",required_argument,NULL,'g'},
		{NULL,0,NULL,0}
	};
	nm = argv[0];
	do{
		nopt = getopt_long(argc,argv,sopts,lopts,NULL);
		switch(nopt){
			case 'f':
				f = 0;
				break;
			case 'h':
				pusage(stdout,0);
				return 0;
			case 'v':
				v = 0;
				break;
			case '?':
				pusage(stderr,1);
				return 0;
			case 'g':
				g = 0;
				break;
			case -1:
				printf("case -1. optind: %d optarg: %s\n",optind,optarg);
				break;
			default:
				printf("optarg: %s\n",optarg);
				abort();
		}
	}while(nopt != -1);

	if(v == 0)
		printf("%s\n","v flag is set");
	if(f == 0)
		printf("%s\n","f flag is set");
	if(g == 0)
		printf("%s\n","g flag is set");
	if(optind < argc){
		printf("supplied argument at: %d %s\n",optind,argv[optind]);
		size_t i;
		for(i = 0; i < argc;i++)
			printf("argv[%zu]: %s\n",i,argv[i]);
		while(optind < argc)
			printf("optind: %d vararg: %s\n",optind,argv[optind++]);
	}
	return 0;
}

