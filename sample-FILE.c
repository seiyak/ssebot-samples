#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

void *file(void *arg)
{
	char *f = (char *)arg;
	FILE *fp = fopen(f,"r");
	printf("fp at: %p f: %s on thread: %ld\n",
		fp,f,pthread_self());
	if(fclose(fp) < 0){
		char buf[4096];
		memset(buf,0,4096);
		strerror_r(errno,buf,4096);
		printf("fclose() error, %s on thread: %ld\n",
			buf,pthread_self());
	}
	return NULL;
}

int main()
{
	size_t i,SZ = 4;
	pthread_t ids[SZ];
	char *files[SZ];
	files[0] = "/home/seiyak/Documents/samples/sample-read.c";
	files[1] = "/home/seiyak/Documents/samples/sample-pipe2.c";
	files[2] = "/home/seiyak/Documents/samples/sample.c";
	files[3] = "/home/seiyak/Documents/samples/sample-array.c";
	for(i = 0; i < SZ;i++)
		pthread_create(&ids[i],NULL,file,(void *)files[i]);
	for(i = 0; i < SZ;i++)
		pthread_join(ids[i],NULL);
	return 0;
}
