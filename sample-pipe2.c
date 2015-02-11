#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

void *ppe(void *arg)
{
	int *p = (int *)malloc(sizeof(int) * 2);
	int res = pipe2(p,0);
	char buf[4095];
	memset(buf,0,4095);
	if(res == -1){
		strerror_r(errno,buf,4095);
		printf("pipe2() error: %s\n",buf);
	}
	printf("res: %d p[0]: %d p[1]: %d on thread: %ld\n",
		res,p[0],p[1],pthread_self());
	if(close(p[0]) < 0){
		memset(buf,0,4095);
		strerror_r(errno,buf,4095);
		printf("close(p[0] error: %s\n",buf);
	}
	if(close(p[1]) < 0){
		memset(buf,0,4095);
		strerror_r(errno,buf,4095);
		printf("close(p[1] error: %s\n",buf);
	}
	free(p);
	return NULL;
}

int main()
{
	size_t i,SZ = 3;
	pthread_t ids[SZ];
	for(i = 0; i < SZ;i++)
		pthread_create(&ids[i],NULL,ppe,NULL);
	for(i = 0; i < SZ;i++)
		pthread_join(ids[i],NULL);
	return 0;
}
