#include <stdio.h>

int main()
{
	char *a = "hello";
	printf("a: %s\n",a);
	a[0] = 'H';
	printf("a: %s\n",a);
	const char *ac = "OK";
	//// as expected, the line below
	//// get read-only error.
	//// sample-constchar.c:10:2: error: assignment of read-only location ‘*ac’.
	//// by gcc (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3.
	ac[0] = 'o';
	printf("ac: %s\n",ac);
	return 0;
}
