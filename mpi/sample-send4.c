#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc,char **argv)
{
	MPI_Init(&argc,&argv);
	int sz,rnk,res,cnt;
	MPI_Comm_rank(MPI_COMM_WORLD,&rnk);
	MPI_Comm_size(MPI_COMM_WORLD,&sz);
	if(rnk == 0){
		char *msg = "msg from rnk0";
		size_t i;
		//// MPI_Scatter() like functionality.
		for(i = 0; i < sz;i++){
			if(i != 0){
				printf("rnk0 about to send to rnk%zu\n",i);
				res = MPI_Send(msg,strlen(msg),MPI_CHAR,i,0,
				MPI_COMM_WORLD);
			}
		}
		for(i = 0; i < sz;i++){
			if(i == 0)
				continue;
			MPI_Status st;
			res = MPI_Probe(i,MPI_ANY_TAG,MPI_COMM_WORLD,&st);
			res = MPI_Get_count(&st,MPI_CHAR,&cnt);
			char *buf = (char *)malloc(sizeof(char) * (cnt + 1));
			res = MPI_Recv(buf,cnt,MPI_CHAR,i,MPI_ANY_TAG,MPI_COMM_WORLD,
				MPI_STATUS_IGNORE);
			buf[cnt] = '\0';
			printf("rnk0 received %d bytes from rnk%zu, %s\n",cnt,i,buf);
			free(buf);
		}
	}else{
		MPI_Status st;
		res = MPI_Probe(0,MPI_ANY_TAG,MPI_COMM_WORLD,&st);
		res = MPI_Get_count(&st,MPI_CHAR,&cnt);
		char *buf = (char *)malloc(sizeof(char) * (cnt + 1));
		res = MPI_Recv(buf,cnt,MPI_CHAR,0,0,MPI_COMM_WORLD,
			MPI_STATUS_IGNORE);
		buf[cnt] = '\0';
		printf("rnk: %d received %d bytes, %s\n",rnk,cnt,buf);
		printf("about to send back from rnk%d\n",rnk);
		free(buf);
		buf = "accepted";
		res = MPI_Send(buf,strlen(buf),MPI_CHAR,0,rnk,MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}
