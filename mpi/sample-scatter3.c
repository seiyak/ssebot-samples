#include <mpi.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc,char **argv)
{
	MPI_Init(&argc,&argv);
	int rnk = -1,sz = -1;
	MPI_Comm_rank(MPI_COMM_WORLD,&rnk);
	MPI_Comm_size(MPI_COMM_WORLD,&sz);
	size_t csz = 1;
	char *c = (char *)malloc(sizeof(char) * csz);
	c[0] = (char)(65 + rnk);
	char *chars = NULL;
	if(rnk == 0)
		chars = (char *)malloc(sizeof(char) * ((csz * sz) + 1));
	int res = MPI_Gather(c,csz,MPI_CHAR,chars,csz,MPI_CHAR,0,
		MPI_COMM_WORLD);
	printf("res from MPI_Gather(): %d on rank: %d\n",res,rnk);
	free(c);
	c = NULL;
	if(rnk == 0){
		chars[csz * sz] = '\0';
		printf("gatherd char is: %s\n",chars);
		free(chars);
		chars = NULL;
	}
	MPI_Finalize();
	return 0;
}
