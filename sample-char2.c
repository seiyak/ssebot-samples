#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void tolwr(char *chars,size_t strt,size_t end)
{
	if(chars == NULL || strt >= end)
		return;
	size_t i;
	for(i = strt;i < end;i++)
		chars[i] = tolower(chars[i]);
}

void toupr(char *chars,size_t strt,size_t end)
{
	if(chars == NULL || strt >= end)
		return;
	size_t i;
	for(i = strt;i < end;i++)
		chars[i] = toupper(chars[i]);
}

int main()
{
	size_t SZ = 10;
	char *chars = (char *)malloc(sizeof(char) * (SZ + 1));
	chars[0] = 'H';
	chars[1] = 'E';
	chars[2] = 'L';
	chars[3] = 'L';
	chars[4] = 'o';
	chars[5] = 'w';
	chars[6] = 'o';
	chars[7] = 'r';
	chars[8] = 'l';
	chars[9] = 'd';
	chars[10] = '\0';

	printf("chars: %s\n",chars);
	tolwr(chars,0,SZ);
	printf("chars again: %s\n",chars);
	toupr(chars,6,SZ);
	printf("chars again again: %s\n",chars);
	free(chars);
	return 0;
}
