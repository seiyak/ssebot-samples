#include <unistd.h>
#include <stdio.h>

int main()
{
	int pid = fork(),res = -1;
	if(pid >= 0){
		if(pid == 0){
			printf("%s\n","pid == 0,in child process");
			printf("%d from execv()\n",execlp("gedit",
				"/home/seiyak/Documents/sample.c","/home/seiyak/Documents/comp.c",NULL));
		}
	}else
		printf("%s\n","fork() got error.");
	return 0;
}
