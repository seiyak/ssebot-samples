#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#define LSTN_BLG 50
#define DEPLLP_TMOT 3000
#define DEPLLP_MXT 3
#define DEPLLP_MXEVNTS 10

int socknonb_v4()
{
	int sfd = socket(AF_INET,SOCK_STREAM | SOCK_NONBLOCK,0);
	if(sfd == -1){
		perror("socket()");
		return -2;
	}
	int y = 1;
	if(setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&y,sizeof(int)) != 0){
		perror("setsockopt()");
		return -1;
	}
	return sfd;
}

int bind_v4(int sfd,struct sockaddr_in *addr,const char *hst,unsigned short prt)
{
	memset(addr,0,sizeof(struct sockaddr_in));
	addr->sin_family = AF_INET;
	addr->sin_port = htons(prt);
	if(hst != NULL)
		inet_pton(addr->sin_family,hst, &(addr->sin_addr));
	if(bind(sfd,(struct sockaddr *)addr,sizeof(struct sockaddr_in)) != 0){
		perror("bind()");
		return -1;
	}
	return 0;
}

int socktlstn_v4(struct sockaddr_in *addr,const char *hst,unsigned short prt)
{
	int sfd = socknonb_v4();
	if(sfd < 0)
		return sfd;
	if(bind_v4(sfd,addr,hst,prt) != 0)
		return -2;
	if(listen(sfd,LSTN_BLG) != 0){
		perror("listen()");
		return -1;
	}
	return sfd;
}

const char *SVR_HST = "192.168.0.100";	//// "localhost"
unsigned short PRT = 6666;
const char *PRT_CHR = "6666";
size_t SLP_TM = 2;
size_t BUF_SZ = 500;

struct param{
	uint8_t k;
	int fd;
};

int gaddrcn(const char *hst,const char *srvc,int ipv,uint8_t k)
{
	if(hst == NULL || srvc == NULL)
		return -4;
	else if(ipv != 4 && ipv != 6)
		return -3;

	struct addrinfo hnt;
	memset(&hnt,0,sizeof(struct addrinfo));
	if(ipv == 4)
		hnt.ai_family = AF_INET;
	else if(ipv == 6)
		hnt.ai_family = AF_INET6;
	hnt.ai_socktype = SOCK_STREAM;
	hnt.ai_flags = AI_CANONNAME;

	struct addrinfo *ai_h = NULL,*ai = NULL;
	int res,fd = getaddrinfo(hst,srvc,&hnt,&ai_h);
	if(fd != 0){
		perror("getaddrinfo()");
		return -2;
	}
	fd = -1;
	size_t cnt = 0;
	for(ai = ai_h;ai != NULL; ai = ai->ai_next){
		cnt++;
		if(fd == -1){
			printf("about to call socket in %s\n",__func__);
			fd = socket(ai->ai_family,ai->ai_socktype,
					ai->ai_protocol);
			perror("socket()");
			if(fd == -1)
				continue;
		}
		if(fd > 0){
			printf("about to call connect() in %s\n",__func__);
			if((res = connect(fd,ai->ai_addr,ai->ai_addrlen)) < 0){
				perror("connect()");
				close(fd);
				fd = -1;
				continue;
			}else{
				perror("connect()");
				break;
			}
		}
	}
	freeaddrinfo(ai_h);
	printf("total # of loop in getaddrinfo() in %s: %zu\n",__func__,cnt);
	return fd;
}

void *strt_client(void *arg)
{
	printf("about to sleep %zu sec in %s\n",SLP_TM,__func__);
	sleep(SLP_TM);
	int sfd = gaddrcn(SVR_HST,PRT_CHR,4,0);
	char *buf = "hello world";
	printf("about to call write() in %s on sfd: %d\n",__func__,sfd);
	int res = write(sfd,buf,strlen(buf));
	perror("write()");
	close(sfd);
	pthread_exit(NULL);
}

int main()
{
	int eplfd = epoll_create1(0);
	if(eplfd < 0){
		perror("epoll_create1() error");
		return -1;
	}

	//// create server socket.
	struct sockaddr_in ad;
	int sfd = socktlstn_v4(&ad,SVR_HST,PRT);
	printf("sfd in %s: %d\n",__func__,sfd);

	//// create data.
	struct param p;
	p.k = 1;
	p.fd = sfd;

	//// register server socket to epoll.
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET;
	ev.data.ptr = &p;
	if(epoll_ctl(eplfd,EPOLL_CTL_ADD,p.fd,&ev) != 0){
		perror("epoll_ctl() error");
		return -1;
	}

	//// create and start client.
	printf("about to create and start client thread in %s\n",__func__);
	pthread_t pid;
	pthread_create(&pid,NULL,strt_client,NULL);

	//// epoll event loop.
	struct epoll_event evnts[DEPLLP_MXEVNTS];
	int n = -1;
	size_t i,cnt = 0;
	while(1){
		n = epoll_wait(eplfd,evnts,DEPLLP_MXEVNTS,-1);
		if(n == -1){
			perror("epoll_wait() error");
			break;
		}else if(n == 0){
			printf("time out %d sec expired\n",DEPLLP_TMOT / 1000);
			break;
		}
		for(i = 0; i < n;i++){
			struct param *p = evnts[i].data.ptr;
			//// find listen fd. accept it.
			if(p->k == 1){
				printf("about to accept listen fd: %d in %s\n",
					p->fd,__func__);
				struct sockaddr_in ad;
				socklen_t adlen = sizeof(struct sockaddr_in);
				//// accept listen sockfd.
				int cfd = accept4(p->fd,(struct sockaddr *)&ad,
					&adlen,SOCK_NONBLOCK);
				if(cfd == -1){
					perror("accept4()");
					break;
				}
				printf("about to reigster connected fd: %d in %s\n",
					cfd,__func__);
				struct param np;
				np.k = 2;
				np.fd = cfd;
				struct epoll_event nev;
				nev.events = EPOLLIN | EPOLLET;
				nev.data.ptr = &np;
				if(epoll_ctl(eplfd,EPOLL_CTL_ADD,np.fd,&nev)
					!= 0){
					perror("epoll_ctl()");
					break;
				}
				printf("done with registering connected fd in %s\n",__func__);
			}else if(p->k == 2){
				char buf[BUF_SZ];
				ssize_t n;
				size_t i;
				printf("about to call read() in %s\n",__func__);
				while((n = read(p->fd,buf,BUF_SZ)) > 0){
					for(i = 0; i < n;i++)
						printf("%c",buf[i]);
					printf("%s","\n");
				}
				if(buf[0] = 'G' && buf[1] == 'E' && buf[2] == 'T'){
					char *temp = "HTTP/1.1 200 OK\r\nHost: 192.168.0.100:6666\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nYou are connected";
					printf("\nabout to write response: %s\n",temp);
					printf("res from write() on fd: %d is: %zd\n",p->fd,write(p->fd,temp,strlen(temp)));
				}

				perror("read()");
			}
		}
	}
	close(eplfd);
	pthread_join(pid,NULL);
	return 0;
}
