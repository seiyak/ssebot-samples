#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define BUF_LEN 4095

void *work(void *arg)
{
	char buf[BUF_LEN + 1];
	memset(buf,0,BUF_LEN + 1);
	if(buf[1] == 'e')
		printf("buf[%d] is 'e' on thread: %ld\n",
			1,pthread_self());
	return NULL;
}

int main()
{
	size_t SZ = 4,i;
	pthread_t ids[SZ];
	for(i = 0; i < SZ;i++)
		pthread_create(&ids[i],NULL,work,NULL);
	for(i = 0; i < SZ;i++)
		pthread_join(ids[i],NULL);
	return 0;
}
