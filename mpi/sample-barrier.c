#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <mpi.h>

int RRNK = 0;

void do4(int rnk)
{
	char *msg = "four";
	char *each = (char *)malloc(sizeof(char) * 2);
	MPI_Scatter(msg,1,MPI_CHAR,each,1,MPI_CHAR,rnk,MPI_COMM_WORLD);
	each[1] = '\0';
	printf("msg is: %s\n",each);
	free(each);
}

void do3(int rnk)
{
	printf("done do3 by rnk: %d\n",rnk);
}

void do2(int rnk)
{
	printf("done do2 by rnk: %d\n",rnk);
	do3(rnk);
}

void do1(int rnk)
{
	printf("done do1 by rnk: %d\n",rnk);
	do2(rnk);
}

int main(int argc,char **argv)
{
	int rnk,sz;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&sz);
	MPI_Comm_rank(MPI_COMM_WORLD,&rnk);
	if(rnk != RRNK){
		int count = 10;
		while(--count > 0){
			printf("rank,%d != RRNK: %d count: %d\n",
				rnk,RRNK,count);
			sleep(1);
		}
	}
	//MPI_Barrier(MPI_COMM_WORLD);
	int cnt = 5;
	if(rnk == 0){
		while(--cnt > 0){
			printf("rnk: %d cnt: %d got bored to wait for others ...\n",
				rnk,cnt);
			sleep(1);
		}
		printf("%s\n","still have time, do something else!!");
		do1(rnk);
	}
	do4(rnk);
	printf("rank: %d about to out ...\n",rnk);
	MPI_Finalize();
	return 0;
}
