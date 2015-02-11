#include <stdio.h>
#include <time.h>

int main()
{
	struct tm tmm;
	time_t t = time(NULL);
	struct tm *tmp = gmtime(&t);
	char buf[30];
	size_t sz = strftime(buf,99,"%a, %d %h %G %T GMT",tmp);
	buf[sz] = '\0';
	printf("current time,sz: %zu is: %s\n",sz,buf);
	return 0;
}
