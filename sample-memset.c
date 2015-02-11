#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	size_t SZ = 10;
	int *fds = (int *)malloc(sizeof(int) * 10);
	memset(fds,-1,sizeof(int) * SZ);
	size_t i;
	for(i = 0; i < SZ;i++)
		printf("i: %zu fds[%zu]: %d\n",i,i,fds[i]);
	free(fds);
	return 0;
}
