#include <stdlib.h>

void set_m(char **chars)
{
	*chars = (char *)malloc(sizeof(char) * 3);
	(*chars)[0] = 'h';
	(*chars)[1] = 'i';
	(*chars)[2] = '\0';
}

int main()
{
	char *chars = NULL;
	set_m(&chars);
	free(chars);
	set_m(NULL);
	return 0;
}
