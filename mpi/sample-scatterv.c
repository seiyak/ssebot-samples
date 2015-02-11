#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void getUrls(char **urls,int **szs,int *tl)
{
	size_t i,j,SZ = 10;
	char *buf[SZ];
	buf[0] = "sample.com";
	buf[1] = "sample.com/login.jsp";
	buf[2] = "sample.com/payment.pl";
	buf[3] = "sample.com/error.jsp";
	buf[4] = "sample2.com";
	buf[5] = "sample2.com/register.asp";
	buf[6] = "sample3.com";
	buf[7] = "sample3.com/show";
	buf[8] = "sample3.com/this";
	buf[9] = "sample4.com";
	if(*szs != NULL)
		free(*szs);
	*szs = (int *)calloc(SZ,sizeof(int));
	*tl = 0;
	for(i = 0; i < SZ;i++){
		(*szs)[i] = strlen(buf[i]);
		(*tl) += (*szs)[i];
	}
	if(*urls != NULL)
		free(*urls);
	*urls = (char *)malloc(sizeof(char) * (*tl));
	size_t of = 0;
	for(i = 0; i < SZ;i++){
		memcpy((*urls) + of,buf[i],(*szs)[i]);
		of += (*szs)[i];
	}
}

int *getDisp(int *szs)
{
	int *disp = (int *)calloc(5,sizeof(int));
	disp[0] = disp[1] = 0;
	disp[2] = szs[0] + szs[1] + szs[2] + szs[3];
	disp[3] = disp[2] + szs[4] + szs[5];
	disp[4] = disp[3] + szs[6] + szs[7] + szs[8];
	return disp;
}

int *getSz(int *szs)
{
	int *s = (int *)calloc(5,sizeof(int));
	s[0] = 0;
	s[1] = szs[0] + szs[1] + szs[2] + szs[3];
	s[2] = szs[4] + szs[5];
	s[3] = szs[6] + szs[7] + szs[8];
	s[4] = szs[9];
	return s;
}

int main(int argc,char **argv)
{
	MPI_Init(&argc,&argv);
	int rnk = -1,sz = -1;
	MPI_Comm_rank(MPI_COMM_WORLD,&rnk);
	MPI_Comm_size(MPI_COMM_WORLD,&sz);
	char *urls = NULL;
	int *szs = NULL;
	int tl = 0;
	getUrls(&urls,&szs,&tl);
	int *disp = getDisp(szs);
	int *szz = getSz(szs);
	if(rnk == 0){
		size_t i;
		printf("disp is like below.\n");
		for(i = 0; i < 5;i++)
			printf("%d ",disp[i]);
		printf("%s","\n");
		printf("%s\n","urls are like below");
		for(i = 0; i < tl;i++)
			printf("%c",urls[i]);
		printf("%s","\n\n");
	}
	char *each = (char *)calloc(szz[rnk] + 1,sizeof(char));
	MPI_Scatterv(urls,szz,disp,MPI_CHAR,each,szz[rnk],MPI_CHAR,0,
		MPI_COMM_WORLD);
	each[szz[rnk]] = '\0';
	printf("rank: %d each size: %d disp: %d\neach:%s\n",rnk,szz[rnk],
		disp[rnk],each);
	free(each);
	each = NULL;
	free(szs);
	szs = NULL;
	free(urls);
	urls = NULL;
	free(disp);
	disp = NULL;
	free(szz);
	szz = NULL;
	MPI_Finalize();
	return 0;
}
