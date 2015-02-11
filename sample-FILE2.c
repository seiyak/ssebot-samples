#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

const char *pth = "/tmp/sample";
const char *msg = "hello world";

void *file(void *arg)
{
	FILE *fp = fopen(pth,"w");
	fwrite(msg,sizeof(char),strlen(msg),fp);
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
	size_t i,SZ = 8;
	pthread_t ids[SZ];
	for(i = 0; i < SZ;i++)
		pthread_create(&ids[i],NULL,file,NULL);
	for(i = 0; i < SZ;i++)
		pthread_join(ids[i],NULL);
	printf("done with threads in %s\n",__func__);
	FILE *fp = fopen(pth,"r");
	if(fp == NULL){
		printf("fopen() error in %s\n",__func__);
		return -1;
	}
	char buf[12];
	size_t n = fread(buf,sizeof(char),12,fp);
	buf[11] = '\0';
	printf("n: %zu buf: %s in %s\n",n,buf,__func__);
	fclose(fp);
	return 0;
}
