#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	int fd1 = open("./sample-fd.c",O_RDONLY);
	int fd2 = open("./sample-fd.c",O_RDONLY);
	int fd3 = open("./sample-fd.c",O_RDONLY);

	printf("fd1: %d fd2: %d fd3: %d\n",fd1,fd2,fd3);
	close(fd1);
	close(fd2);
	close(fd3);
	return 0;
}
