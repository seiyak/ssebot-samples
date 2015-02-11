#include <pthread.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <stdlib.h>

void *strt_another(void *arg)
{
	int cnt = 0;
	while(1){
		if(++cnt == 3){
			printf("about to break while loop after %d seconds on thread %ld\n",
				cnt,pthread_self());
			break;
		}
		sleep(1);
	}
	int *v = (int *)malloc(sizeof(int));
	*v = 0;
	pthread_exit(v);
}


void *strt_th(void *arg)
{
	pthread_t tid;
	pthread_create(&tid,NULL,strt_another,NULL);
	printf("about to wait for thread %ld\n",tid);
	pthread_join(tid,NULL);
	printf("out of thread %ld\n",pthread_self());
	pthread_exit(NULL);
}

int main()
{
	int dn = -1;
	pthread_t pid;
	pthread_create(&pid,NULL,strt_th,NULL);
	printf("about to wait thread id %ld\n",pid);
	pthread_join(pid,NULL);
	printf("%s\n","out of main()");
	return 0;
}
