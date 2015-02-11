#include <stdio.h>
#include <string.h>

int main()
{
	size_t i,sz = 10;
	int ar[sz];
	memset(ar,0,sizeof(int) * sz);
	for(i = 0; i < sz;i++)
		printf("i: %zu val: %d\n",i,ar[i]);
	return 0;
}
