#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc,char **argv)
{
	/*
	 * MPI_Prove() is a busy wait and consumes 100% CPU to only wait messages but
	 * doing nothing!!!
	 */
	MPI_Init(&argc,&argv);
	int rnk,sz,cnt,res = -1;
	MPI_Comm_rank(MPI_COMM_WORLD,&rnk);
	MPI_Comm_size(MPI_COMM_WORLD,&sz);
	if(rnk == 0){
		/*
		int c = 10;
		while(--c >= 0){
			printf("count down, %d\n",c);
			sleep(1);
		}
		printf("%s\n","about to call MPI_Send()");
		char *msg = "hello from rnk0";
		res = MPI_Send(msg,strlen(msg),MPI_CHAR,1,0,MPI_COMM_WORLD);
		*/
	}else if(rnk == 1){
		MPI_Status st;
		MPI_Request req;
		//res = MPI_Probe(0,0,MPI_COMM_WORLD,&st);
		printf("rnk: %d about to call MPI_Wait()\n",rnk);
		char *buf = NULL;
		res = MPI_Irecv(buf,0,MPI_CHAR,0,0,MPI_COMM_WORLD,&req);
		res = MPI_Wait(&req,&st);
		res = MPI_Get_count(&st,MPI_CHAR,&cnt);
		buf = (char *)malloc(sizeof(char) * (cnt + 1));
		res = MPI_Recv(buf,cnt,MPI_CHAR,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		buf[cnt] = '\0';
		printf("rnk: %d received %d bytes, %s\n",rnk,cnt,buf);
		free(buf);
	}
	printf("rnk: %d has nothing to do\n",rnk);
	MPI_Finalize();
	return 0;
}
