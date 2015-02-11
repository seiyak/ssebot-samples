#include <stdio.h>

struct cpust{
	unsigned long utime;
	unsigned long stime;
	unsigned long um;
	unsigned long nm;
	unsigned long sm;
};


struct cpusg{
	float u;
	float s;
};

void gcpusg(struct cpusg *usg,struct cpust *st1,struct cpust *st2)
{
	//if(st1->utime == st2->utime)
	//	usg->u = 0;
	//else{
		if(st2->um == st1->um)
			usg->u = (st2->utime - st1->utime) / 100;
		else{
			printf("st2->utime: %ld st1->utime: %ld st2->um: %ld st1->um: %ld\n",
				st2->utime,st1->utime,st2->um,st1->um);
			usg->u = (st2->utime - st1->utime) / (float)((st2->um - st1->um) * 100);
		}
	//}
	//if(st1->stime == st2->stime)
	//	usg->s = 0;
	//else{
		if(st2->sm == st1->sm)
			usg->s = (st2->stime - st1->stime) / 100;
		else
			usg->s = (st2->stime - st1->stime) / (float)((st2->sm - st1->sm) * 100);
	//}
}

void clear_cpust(struct cpust *st)
{
	st->utime = 0;
	st->stime = 0;
	st->um = 0;
	st->nm = 0;
	st->sm = 0;
}

void read_pid_st(struct cpust *st)
{
	FILE *fp;
	fp = fopen("/proc/30634/stat","r");
	int pid,ppid,pgrp,session,tty_nr,tpgid;
	unsigned int flags;
	unsigned long minflt,cminflt,majflt,cmajflt;
	char cm[255],cpun[255],stat;
	if(fscanf(fp,"%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu",
		&pid,cm,&stat,&ppid,&pgrp,&session,&tty_nr,&tpgid,&flags,
		&minflt,&cminflt,&majflt,&cmajflt,&(st->utime),&(st->stime)) == EOF)
		perror("fscanf");
	printf("pid: %d command name: %s utime: %ld stime: %ld\n",
		pid,cm,st->utime,st->stime);
	fclose(fp);
}

void read_st(struct cpust *st)
{
	char cpun[255];
	FILE *fp = fopen("/proc/stat","r");
	if(fscanf(fp,"%s %ld %ld %ld",cpun,&(st->um),&(st->nm),&(st->sm)) == EOF)
		perror("fscanf");
	printf("cpu: %s user mode: %ld nice mode: %ld system mode: %ld\n",
		cpun,st->um,st->nm,st->sm);
	fclose(fp);
}

int main()
{/*
	FILE *fp;
	fp = fopen("/proc/4184/stat","r");
	int pid,ppid,pgrp,session,tty_nr,tpgid;
	unsigned int flags;
	unsigned long minflt,cminflt,majflt,cmajflt,utime,stime,um,nm,sm;
	char cm[255],cpun[255],stat;
	if(fscanf(fp,"%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu",
		&pid,cm,&stat,&ppid,&pgrp,&session,&tty_nr,&tpgid,&flags,
		&minflt,&cminflt,&majflt,&cmajflt,&utime,&stime) == EOF)
		perror("fscanf");
	printf("pid: %d command name: %s utime: %ld stime: %ld\n",
		pid,cm,utime,stime);
	fclose(fp);
	fp = fopen("/proc/stat","r");
	if(fscanf(fp,"%s %ld %ld %ld",cpun,&um,&nm,&sm) == EOF)
		perror("fscanf");
	printf("cpu: %s user mode: %ld nice mode: %ld system mode: %ld\n",
		cpun,um,nm,sm);
	fclose(fp);
	return 0;
*/
	size_t cnt = 0;
	struct cpust st1,st2;
	struct cpusg usg;
	while(cnt++ < 20){
		read_pid_st(&st1);
		read_st(&st1);
		sleep(1);
		read_pid_st(&st2);
		read_st(&st2);
		gcpusg(&usg,&st1,&st2);
		printf("cnt: %zu user mode: %f %% system mode: %f %%\n\n",
			cnt,usg.u,usg.s);
	}
}
