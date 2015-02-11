#include <stdio.h>
#include <stdlib.h>

char *substr(char *a,size_t l,char *b,size_t strt,size_t end)
{
	size_t i,of = strt;
	for(i = strt - of;i < end - of;i++)
		b[i] = a[i];
	b[i] = '\0';
	return b;
}

int main()
{
	size_t SZ = 10;
	char *a = (char *)malloc(sizeof(char) * (SZ + 1));
	a[0] = 'a';
	a[1] = 'b';
	a[2] = 'c';
	a[3] = 'd';
	a[4] = 'e';
	a[5] = 'f';
	a[6] = 'g';
	a[7] = 'h';
	a[8] = 'i';
	a[9] = 'j';
	a[10] = '\0';
	size_t strt = 2,end = 8;
	char b[(end - strt) + 1];
	printf("substring: b,%s from %zu to %zu\n",substr(a,SZ,b,strt,end),strt,end);
	free(a);
	return 0;
}
