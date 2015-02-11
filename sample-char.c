#include <stdio.h>

int main()
{
	printf("NUL: %d %c\n",(int)'\0','\0');
	printf("a: %d %c\n",(int)'a','a');
	printf("h: %d m: %d (int)'h' + (int)'m': %d\n",
		(int)'h',(int)'m',(int)'h' + (int)'m');
	return 0;
}
