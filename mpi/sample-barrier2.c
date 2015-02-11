#include <mpi.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void snd(int rnk)
{
	if(rnk != 0)
		return;
	char *msg = "four";
	char *each = (char *)malloc(sizeof(char) * 2);
	MPI_Scatter(msg,1,MPI_CHAR,each,1,MPI_CHAR,0,MPI_COMM_WORLD);
	MPI_Scatter(msg,1,MPI_CHAR,each,1,MPI_CHAR,1,MPI_COMM_WORLD);
	MPI_Scatter(msg,1,MPI_CHAR,each,1,MPI_CHAR,2,MPI_COMM_WORLD);
	MPI_Scatter(msg,1,MPI_CHAR,each,1,MPI_CHAR,3,MPI_COMM_WORLD);

	each[1] = '\0';
	printf("rnk: %d each: %s\n",rnk,each);
}

int main(int argc,char **argv)
{
	MPI_Init(&argc,&argv);
	int rnk,sz;
	MPI_Comm_size(MPI_COMM_WORLD,&sz);
	MPI_Comm_rank(MPI_COMM_WORLD,&rnk);
	if(rnk != 0){
		int cnt = 5;
		while(--cnt > 0){
			printf("waisting time here, cnt: %d rnk: %d\n",cnt,rnk);
			sleep(1);
		}
		printf("done with wasting time,rnk: %d\n",rnk);
	}
	snd(rnk);
	//MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;
}
