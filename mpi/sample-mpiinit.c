#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *runMPI(int rnk,int rrnk)
{
	char *str = "hello world";
	char *chars = NULL;
	if(rnk == rrnk){
		chars = (char *)malloc(sizeof(char) * (strlen(str) + 1));
		memcpy(chars,str,strlen(str));
		chars[strlen(str)] = '\0';
	}
	return chars;
}

int main(int argc,char **argv)
{
	MPI_Init(&argc,&argv);
	int rnk = -1,sz = -1;
	MPI_Comm_rank(MPI_COMM_WORLD,&rnk);
	MPI_Comm_size(MPI_COMM_WORLD,&sz);
	if(rnk == 0)
		printf("before runMPI() from rank: %d\n",rnk);
	char *chars = runMPI(rnk,0);
	printf("chars: %s from rank: %d\n",chars,rnk);
	if(rnk == 0)
		printf("after runMPI() from rank: %d\n",rnk);
	MPI_Finalize();
	return 0;
}
