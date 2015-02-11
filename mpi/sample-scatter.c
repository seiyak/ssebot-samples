#include <mpi.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>

char *ghn()
{
	char hst[NI_MAXHOST];
	struct ifaddrs *ifa = NULL,*ifaddr = NULL;
	char *n = NULL;
	int res = getifaddrs(&ifaddr);
	if(res < 0)
		perror("getifaddrs():");
	for(ifa = ifaddr;ifa != NULL;ifa = ifa->ifa_next){
		if (ifa->ifa_addr->sa_family == AF_INET || ifa->ifa_addr->sa_family == AF_INET6) {
                   res = getnameinfo(ifa->ifa_addr,
                           (ifa->ifa_addr->sa_family == AF_INET) ? sizeof(struct sockaddr_in) :
                                                 sizeof(struct sockaddr_in6),
                           hst, NI_MAXHOST,
                           NULL, 0, NI_NUMERICHOST);
                   if (res != 0) {
                       printf("name: %s getnameinfo() failed: %s\n",ifa->ifa_name,gai_strerror(res));
                       break;
                   }
		   if(ifa->ifa_flags & IFF_UP){
                       //printf("name: %s up\t\taddress: <%s>\n",ifa->ifa_name,hst);
		       if((ifa->ifa_flags & IFF_LOOPBACK) == 0 && (ifa->ifa_addr->sa_family == AF_INET)){
				n = (char *)malloc(sizeof(char) * 15);
				memset(n,32,15);
				memcpy(n,hst,strlen(hst));
				//n[strlen(hst)] = '\0';
				break;
		       }
		   }
		}
		//else
                // printf("name: %s down\t\taddress: <%s> len: %zu %d\n",ifa->ifa_name,hst,
		//	strlen(hst),(ifa->ifa_name == NULL));
	}
	freeifaddrs(ifaddr);
	return n;
}

int main(int argc,char **argv)
{
	MPI_Init(&argc,&argv);
	int rnk = -1,sz = -1;
	MPI_Comm_rank(MPI_COMM_WORLD,&rnk);
	MPI_Comm_size(MPI_COMM_WORLD,&sz);
	size_t SZ = sz;
	char *sb = NULL;
	char *m = "GET /v2s-hn HTTP/1.1\r\nHost:\r\n\r\n";
	size_t i,szm = strlen(m);
	if(rnk == 0){
		sb = (char *)malloc(sizeof(char) * szm * SZ);
		size_t of = 0;
		for(i = 0; i < SZ;i++){
			memcpy(sb + of,m,szm);
			of += szm;
		}
		//for(i = 0; i < (szm * SZ);i++)
		//	printf("%c",sb[i]);
	}
	char *each = (char *)malloc(sizeof(char) * szm);
	MPI_Scatter(sb,szm,MPI_CHAR,each,szm,MPI_CHAR,0,MPI_COMM_WORLD);
	if(rnk == 0){
		free(sb);
		sb = NULL;
	}
	//printf("recevied m: %s size: %zu on rank: %d\n",each,szm,rnk);
	char *hn = ghn();
	size_t SZ_TEMPLATE = 80;
	char *rtn = (char *)malloc(sizeof(char) * (SZ_TEMPLATE + 1));
	char *template = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 15\r\n\r\n";
	//printf("template size: %zu\n",strlen(template));
	memcpy(rtn,template,strlen(template));
	memcpy(rtn + strlen(template),hn,15);
	rtn[SZ_TEMPLATE] = '\0';
	//printf("rtn size: %zu\n",strlen(rtn));
	char *allhns = NULL;
	if(rnk == 0)
		allhns = (char *)malloc(sizeof(char) * (SZ_TEMPLATE * sz));
	MPI_Gather(rtn,SZ_TEMPLATE,MPI_CHAR,allhns,SZ_TEMPLATE,MPI_CHAR,0,MPI_COMM_WORLD);
	if(rnk == 0){
		//for(i = 0; i < (SZ_TEMPLATE * sz);i++)
		//	printf("%c",allhns[i]);
		free(allhns);
		allhns = NULL;
	}
	free(each);
	each = NULL;
	free(hn);
	hn = NULL;
	free(rtn);
	rtn = NULL;
	MPI_Finalize();
	return 0;
}
