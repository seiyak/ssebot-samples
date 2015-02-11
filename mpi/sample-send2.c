#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char **argv)
{
	MPI_Init(&argc,&argv);
	int sz,rnk,res,cnt;
	char *buf = NULL;
	MPI_Comm_size(MPI_COMM_WORLD,&sz);
	MPI_Comm_rank(MPI_COMM_WORLD,&rnk);
	if(rnk == 0){
		char *msg1 = "hello from rnk0 to rnk1";
		printf("%s\n","about to send msg to rnk1");
		res = MPI_Send(msg1,strlen(msg1),MPI_CHAR,1,1,MPI_COMM_WORLD);
		printf("%s\n","about to send msg to rnk3");
		//char *msg3 = "hello from rnk0 to rnk3";
		char *msg3 = NULL;
		res = MPI_Send(NULL,0,MPI_CHAR,3,3,MPI_COMM_WORLD);
	}

	if(rnk == 1 || rnk == 3){
		MPI_Status st;
		res = MPI_Probe(0,rnk,MPI_COMM_WORLD,&st);
		res = MPI_Get_count(&st,MPI_CHAR,&cnt);
		buf = (char *)malloc(sizeof(char) * (cnt + 1));
		res = MPI_Recv(buf,cnt,MPI_CHAR,0,rnk,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		buf[cnt] = '\0';
		printf("rnk: %d received %d bytes, %s\n",rnk,cnt,buf);
		free(buf);
	}
	printf("rnk %d is done\n",rnk);
	MPI_Finalize();
	return 0;
}
