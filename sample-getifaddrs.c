#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <string.h>

int main()
{
	char hst[NI_MAXHOST];
	struct ifaddrs *ifa = NULL,*ifaddr = NULL;
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
		if(ifa->ifa_flags & IFF_UP)
                   printf("name: %s up\t\taddress: <%s>\n",ifa->ifa_name,hst);
		}else
                   printf("name: %s down\t\taddress: <%s> len: %zu %d\n",ifa->ifa_name,hst,
			strlen(hst),(ifa->ifa_name == NULL));
	}
	freeifaddrs(ifaddr);
}
