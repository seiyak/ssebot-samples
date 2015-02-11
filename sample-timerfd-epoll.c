#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MX_EVNTS 10
#define EPL_TOUT 3000
#define MX_CNT 5

struct param{
	struct itimerspec its;
	int tfd;
};

void *strt_eplth(void *arg)
{
	struct epoll_event evnts[MX_EVNTS];
	int *eplfd = (int *)arg;
	int n = -1;
	size_t i,cnt = 0;
	while(1){
		n = epoll_wait(*eplfd,evnts,MX_EVNTS,EPL_TOUT);
		if(n == -1){
			perror("epoll_wait() error");
			break;
		}else if(n == 0){
			printf("time out %d sec expired\n",EPL_TOUT / 1000);
			break;
		}
		for(i = 0; i < n;i++){
			struct param *pm = (struct param *)(evnts[i].data.ptr);
			printf("tfd: %d\ninitial expiration: %ld\ninterval: %ld\n\n",
				pm->tfd,
				(long)(pm->its.it_value.tv_sec),
				(long)(pm->its.it_interval.tv_sec));
			if(epoll_ctl(*eplfd,EPOLL_CTL_DEL,pm->tfd,NULL) != 0){
				perror("epoll_ctl(DEL) error in thread");
				break;
			}
			struct epoll_event ev;
			ev.events = EPOLLIN | EPOLLET;
			pm->its.it_value.tv_sec =
				pm->its.it_value.tv_sec +
				pm->its.it_interval.tv_sec;
			ev.data.ptr = pm;
			if(timerfd_settime(pm->tfd,TFD_TIMER_ABSTIME,&(pm->its),NULL) != 0){
				perror("timerfd_settime() error in thread");
				break;
			}
			if(epoll_ctl(*eplfd,EPOLL_CTL_ADD,pm->tfd,&ev) != 0){
				perror("epoll_ctl(ADD) error in thread");
				break;
			}
		}
		if(++cnt == MX_CNT){
			printf("cnt reached MX_CNT, %d\n",MX_CNT);
			break;
		}
	}
	close(*eplfd);
	pthread_exit(NULL);
}

int create_timerfd(struct itimerspec *its,time_t interval)
{
	int tfd = timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK);
	if(tfd < 0){
		perror("timerfd_create() error");
		return -2;
	}
	struct timespec nw;
	if(clock_gettime(CLOCK_MONOTONIC,&nw) != 0){
		perror("clock_gettime() error");
		return -1;
	}
	its->it_value.tv_sec = nw.tv_sec + interval;
	its->it_value.tv_nsec = 0;
	its->it_interval.tv_sec = interval;
	its->it_interval.tv_nsec = 0;
	return tfd;
}

int main()
{
	time_t INTERVAL = 2;
	struct itimerspec its;
	int tfd = create_timerfd(&its,INTERVAL);
	if(tfd < 0)
		return -1;
	int eplfd = epoll_create1(0);
	if(eplfd < 0){
		perror("epoll_create1() error");
		return -1;
	}
	struct param pm;
	pm.its = its;
	pm.tfd = tfd;
	if(timerfd_settime(pm.tfd,TFD_TIMER_ABSTIME,&(pm.its),NULL) != 0){
		perror("timerfd_settime() error");
		return -1;
	}
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET;
	ev.data.ptr = &pm;
	if(epoll_ctl(eplfd,EPOLL_CTL_ADD,pm.tfd,&ev) != 0){
		perror("epoll_ctl() error");
		return -1;
	}
	pthread_t pid;
	if(pthread_create(&pid,NULL,strt_eplth,(void *)&eplfd) != 0){
		perror("pthread_create() error");
		return -1;
	}

	//// add another timerfd.
	INTERVAL = 1;
	struct itimerspec its2;
	int tfd2 = create_timerfd(&its2,INTERVAL);
	if(tfd2 < 0)
		return -1;
	struct param pm2;
	pm2.its = its2;
	pm2.tfd = tfd2;
	if(timerfd_settime(pm2.tfd,TFD_TIMER_ABSTIME,&(pm2.its),NULL) != 0){
		perror("timerfd_settime() error");
		return -1;
	}
	struct epoll_event ev2;
	ev2.events = EPOLLIN | EPOLLET;
	ev2.data.ptr = &pm2;
	if(epoll_ctl(eplfd,EPOLL_CTL_ADD,pm2.tfd,&ev2) != 0){
		perror("epoll_ctl() error");
		return -1;
	}

	if(pthread_join(pid,NULL) != 0){
		perror("pthread_join() error");
		return -1;
	}
	close(tfd);
	close(tfd2);
	return 0;
}
