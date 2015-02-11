#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc,char **argv)
{
	MPI_Init(&argc,&argv);
	int rnk,sz;
	MPI_Comm_size(MPI_COMM_WORLD,&sz);
	MPI_Comm_rank(MPI_COMM_WORLD,&rnk);
	int res = -1,cnt = -1;
	char *buf = NULL;
	if(rnk == 0){
		char *msg1 = "message to rnk1 from rnk0";
		res = MPI_Send(msg1,strlen(msg1),MPI_CHAR,1,1,MPI_COMM_WORLD);
		printf("res from MPI_Send with msg1: %d\n",res);
		char *msg3 = "message to rnk3 from rnk3";
		res = MPI_Send(msg3,strlen(msg3),MPI_CHAR,3,3,MPI_COMM_WORLD);
		printf("res from MPI_Send with msg1: %d\n",res);

		size_t i;
		int rnks[2] = {1,3};
		MPI_Status st;
		for(i = 0; i < 2;i++){
			res = MPI_Probe(rnks[i],rnks[i],MPI_COMM_WORLD,&st);
			res = MPI_Get_count(&st,MPI_CHAR,&cnt);
			buf = (char *)malloc(sizeof(char) * (cnt + 1));
			res = MPI_Recv(buf,cnt,MPI_CHAR,rnks[i],rnks[i],MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			buf[cnt] = '\0';
			printf("rnk 0: received back msg from rnk: %d cnt: %d : %s\n",rnks[i],cnt,buf);
			free(buf);
		}
		printf("%s\n","rnk0 is done");
	}

	if(rnk == 1 || rnk == 3){
		MPI_Status st;
		res = MPI_Probe(0,rnk,MPI_COMM_WORLD,&st);
		printf("rnk: %d about to receive message\n",rnk);
		res = MPI_Get_count(&st,MPI_CHAR,&cnt);
		buf = (char *)malloc(sizeof(char) * (cnt + 1));
		res = MPI_Recv(buf,cnt,MPI_CHAR,0,rnk,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		buf[cnt] = '\0';
		printf("rnk: %d received msg cnt: %d : %s\n",rnk,cnt,buf);
		printf("rnk: %d about to send back...\n",rnk);
		res = MPI_Send(buf,cnt,MPI_CHAR,0,rnk,MPI_COMM_WORLD);
		free(buf);
	}
	printf("rnk: %d has nothing to do. about to call MPI_Finalize()\n",
		rnk);
	MPI_Finalize();
}
