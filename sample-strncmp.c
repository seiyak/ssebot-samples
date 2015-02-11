#include <string.h>
#include <stdio.h>

int main()
{
	char *buf = "javascript:void(0)";
	char *ptn = "javascript:";

	int res = strncmp(buf,ptn,strlen(ptn));
	printf("res: %d buf: %s ptn: %s cmp size: %zu\n",res,buf,ptn,
		strlen(ptn));

	char *tmp;
	tmp = ptn;
	ptn = buf;
	buf = tmp;
	size_t m;
	if(strlen(ptn) > strlen(buf))
		m = strlen(buf);
	else if(strlen(ptn) < strlen(buf))
		m = strlen(ptn);
	else
		m = strlen(ptn);
	res = strncmp(ptn,buf,m);
	printf("res: %d buf: %s ptn: %s cmp size: %zu\n",res,buf,ptn,m);
	return 0;
}
