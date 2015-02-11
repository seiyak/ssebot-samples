#include <string.h>
#include <stdio.h>

int main()
{
	char *chars = "this is a sample and simple sample";
	char *ptr = chars;
	char *ptn = "sample";
	size_t ptn_l = strlen(ptn);
	char *res = NULL;
	size_t n = 0;
	printf("ptr at: %p\n",ptr);
	while((res = strstr(ptr,ptn)) != NULL){
		printf("res at: %p\n",res);
		ptr = res;
		ptr += ptn_l;
		n++;
	}

	printf("n is %zu in %s\n",n,__func__);
	return 0;
}
