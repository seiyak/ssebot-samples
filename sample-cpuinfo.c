#include <stdio.h>

int main()
{
	FILE *fp;
	fp = fopen("/proc/cpuinfo","r");
	ssize_t n;
	char buf[4095];
	unsigned long id;
	size_t i;
	while((n = fread(buf,sizeof(char),4095,fp)) > 0){
		if(sscanf(buf,"stepping : %ld\n",&id) < 0)
			perror("fscanf");
		printf("found id: %ld\n",id);
		//for(i = 0; i < n;i++)
		//	printf("%c",buf[i]);
		//printf("\n\nn is: %d bytes\n\n",n);
	}
	fclose(fp);
	return 0;
}
