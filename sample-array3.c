#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	size_t SZ = 10;
	char buf[SZ];
	memset(buf,'\0',SZ);
	if(buf[0] == 0)
		printf("buf[0] is 0: %c %d\n",buf[0],buf[0]);
	else
		printf("buf[0]: %c %d\n",buf[0],buf[0]);
	return 0;
}
