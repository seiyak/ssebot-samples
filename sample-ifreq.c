#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
	int sock = socket(AF_INET,SOCK_STREAM,0);
	struct ifreq ifr;
	char *NM = "eth1";
	strncpy(ifr.ifr_name,NM,strlen(NM));
	int res = ioctl(sock,SIOCGIFADDR,&ifr);
	if(res < 0)
		perror("ioctl() error");
	printf("%s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
	close(sock);
	return 0;
}
