#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

#define SZ 4

struct chars{
	size_t sz;
	char **strs;
};

void init_chars(struct chars *chrs,size_t sz)
{
	if(chrs == NULL)
		return;
	chrs->sz = sz;
	chrs->strs = (char **)malloc(sizeof(char *) * sz);
	size_t i;
	for(i = 0; i < sz;i++)
		(chrs->strs)[i] = NULL;
}

int main(int argc,char **argv)
{
	MPI_Init(&argc,&argv);
	int rnk,sz;
	MPI_Comm_size(MPI_COMM_WORLD,&sz);
	MPI_Comm_rank(MPI_COMM_WORLD,&rnk);
	struct chars *chrs = (struct chars *)malloc(sizeof(struct chars));
	if(rnk == 0){
		init_chars(chrs,SZ);
		(chrs->strs)[0] = "hello world 0";
		(chrs->strs)[1] = "hello world 1";
		(chrs->strs)[2] = "hello world 2";
		(chrs->strs)[3] = "hello world 3";
	}
	MPI_Barrier(MPI_COMM_WORLD);
	//// this printf doesn't work when rnk != 0.
	//// struct chars is allocated for all the ranks,
	//// but they didn't call init_chars() but only rank0 did!!
	//// as a result, they get segmentation fault on ranks != 0!!!
	printf("rnk: %d chrs at: %p chrs->strs[%d]: %s\n",
		rnk,chrs,rnk,(chrs->strs)[rnk]);
	if(rnk == 0)
		free(chrs->strs);
	free(chrs);
	MPI_Finalize();
	return 0;
}
