#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

char *read_file(const char *pth,int rnk)
{
	struct stat st;
	int res = stat(pth,&st);
	if(res != 0)
		return NULL;
	printf("pth: %s size: %zu on rank: %d\n",pth,st.st_size,rnk);
	FILE *fp;
	fp = fopen(pth,"r");
	char *chars = (char *)malloc(sizeof(char) * (st.st_size + 1));
	if(chars == NULL)
		return NULL;
	size_t of = 0,sz = 0;
	while((sz = fread(chars + of,sizeof(char),st.st_size,fp)) > 0)
		of += sz;
	fclose(fp);
	chars[st.st_size] = '\0';
	return chars;
}

int main(int argc,char **argv)
{
	MPI_Init(&argc,&argv);
	int rnk = -1,sz = -1;
	MPI_Comm_rank(MPI_COMM_WORLD,&rnk);
	MPI_Comm_size(MPI_COMM_WORLD,&sz);
	size_t content_sz = 66228;
	char *contents = NULL;
	if(rnk == 0){
		contents = (char *)malloc(sizeof(char) *
			(content_sz * sz));
		size_t i;
		char *content = NULL;
		for(i = 0; i < sz;i++){
			content = read_file("/home/seiyak/Documents/sse/test/lib/str/clen_test_data_66228",rnk);
			memcpy(contents + (content_sz * i),content,strlen(content));
			free(content);
		}
	}
	char *each = (char *)malloc(sizeof(char) * (content_sz + 1));
	MPI_Scatter(contents,content_sz,MPI_CHAR,each,content_sz,MPI_CHAR,0,MPI_COMM_WORLD);
	each[content_sz] = '\0';
	//// just check that what MPI_Scatter() scatters is received
	//// in the receive side contiguously in the buffer.
	if(rnk == 2)
		printf("rnk: %d %zu bytes\n%s\n\n",rnk,content_sz,each);
	if(rnk == 0){
		free(contents);
		contents = NULL;
	}
	free(each);
	each = NULL;
	MPI_Finalize();
	return 0;
}
