#include <string.h>
#include <stdio.h>

int main()
{
	char *buf = "hello world";
	char *ptn = "world";
	char *str = strstr(buf,ptn);
	size_t idx;
	if(str != NULL){
		printf("buf at: %p str at: %p\n",buf,str);
		printf("buf: %s str: %s\n",buf,str);
		printf("str: %ld - buf: %ld = %ld\n",(long)str,(long)buf,str - buf);
		printf("buf[str - buf]: %c\n",buf[str - buf]);
	}
	return 0;
}
