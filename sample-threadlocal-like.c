#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

struct a{
	int val;
	void *v;
};

void *th(void *arg)
{
	struct a *aa = (struct a *)arg;
	int cnt = 5;
	while(cnt-- > 0)
		(aa->val)++;
	int *rtn = (int *)malloc(sizeof(int));
	*rtn = aa->val;
	pthread_exit(rtn);
}

int main()
{
	size_t i,SZ = 10;
	int res = -1;
	pthread_t ids[SZ];
	struct a **a_arr = (struct a **)calloc(SZ,sizeof(struct a *));
	for(i = 0; i < SZ;i++){
		a_arr[i] = (struct a *)malloc(sizeof(struct a));
		a_arr[i]->val = 0;
		if((res = pthread_create(&ids[i],NULL,th,(void *)a_arr[i])) != 0)
			perror("pthread_create() error.");
	}
	/*
	 * as expected, this cases a race condition between the main thread and thread
	 * created by create_pthread()
	 */
	////a_arr[2]->val += 2;
	for(i = 0; i < SZ;i++){
		int *rtn = NULL;
		if((res = pthread_join(ids[i],(void **)&rtn)) != 0)
			perror("pthread_join() error.");
		printf("rtn: %d thread id: %ld\n",*rtn,ids[i]);
		free(rtn);
		rtn = NULL;
	}
	for(i = 0; i < SZ;i++)
		free(a_arr[i]);
	free(a_arr);
	return 0;
}
