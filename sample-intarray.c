#include <stdlib.h>
#include <stdio.h>

int main()
{
	size_t SZ = 10;
	int ar[SZ];
	size_t i;
	for(i = 0; i < SZ - 1;i++){
		if(i == SZ - 1)
			ar[i] = '\0';
		else
			ar[i] = i;
	}
	for(i = 0 ; i < SZ;i++)
		printf("i: %zu ar[%zu]: %d\n",i,i,ar[i]);
	return 0;
}
