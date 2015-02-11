#include <stdio.h>
#include <stdlib.h>

int main()
{
	size_t i,SZ = 10;
	int *ar = (int *)calloc(SZ,sizeof(int));
	for(i = 0; i < SZ;i++)
		printf("calloc ar[%zu]: %d at: %p\n",i,ar[i],&(ar[i]));
	free(ar);
	printf("%s","\n");
	int *arr = (int *)malloc(sizeof(int) * SZ);
	for(i = 0; i < SZ;i++)
		printf("malloc ar[%zu]: %d at %p\n",i,arr[i],&(arr[i]));
	free(arr);

	char **chars = (char **)calloc(SZ,sizeof(char *));
	for(i = 0; i < SZ;i++){
		printf("callc chars[%zu]: %s is NULL ?: %d at %p\n",i,chars[i],(chars[i] == NULL),chars[i]);
	}
	free(chars);
	return 0;
}
