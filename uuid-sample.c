#include <uuid/uuid.h>
#include <stdio.h>
#include <pthread.h>

void *gen_uuid(void *args)
{
	uuid_t u1;
	uuid_generate(u1);
	char in[37];
	uuid_unparse(u1,in);
	printf("thread id: %ld, uuid: %s\n",pthread_self(),in);
	pthread_exit(NULL);
}

int main()
{
	size_t i,SZ = 30;
	pthread_t ths[SZ];
	for(i = 0; i < SZ;i++){
		pthread_create(&ths[i],NULL,gen_uuid,NULL);
	}
	for(i = 0; i < SZ;i++){
		pthread_join(ths[i],NULL);
	}
	return 0;
}

