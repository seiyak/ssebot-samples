#include <pthread.h>
#include <stdio.h>
#include <string.h>

#define BUF_LEN 4095

void ar(unsigned long id)
{
	char *buf[BUF_LEN + 1];
	memset(buf,0,BUF_LEN + 1);
	printf("buf at: %p on thread: %ld\n",buf,id);
}

void *array(void *arg)
{
	ar(pthread_self());
	return 0;
}

int main()
{
	size_t i,SZ = 8;
	pthread_t ids[SZ];
	for(i = 0; i < SZ;i++)
		pthread_create(&ids[i],NULL,array,NULL);
	for(i = 0; i < SZ;i++)
		pthread_join(ids[i],NULL);
	return 0;
}
