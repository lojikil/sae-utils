/* Stefan's/Simple connect scanner; wrote this when I
 * was a sysadmin, stuck on a network with no inet
 * access, and needed to scan hosts for open ports.
 * The machine didn't have nc, amap, or nmap, and I
 * didn't have the time to use SneakerNet.
 * Copyright 2011 Stefan Edwards under zlib/png.
 * see the LICENSE file for details
 */
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define nil NULL

int
main(int ac, char **al)
{
	int attack_sock = 0, port_iter = 2,rc = 0;
	char *hoststr = nil, **ports = nil; 
	struct sockaddr_in target;
	struct hostent *host = nil;	
	if(ac < 3)
	{
		printf("Usage: scs [host] [port0 port1 ... portN]\ne.g. scs 192.168.1.40 22 80\n");
		return 0;
	}
	if((host = gethostbyname(al[1])) == nil)
	{
		printf("gethostbyname failed!\n");
		return 1;
	}	
	printf("%%DEBUG: (%s)\n",inet_ntoa(*( struct in_addr*)(host->h_addr_list[0])));	
	target.sin_family = AF_INET;
        target.sin_addr.s_addr = inet_addr(inet_ntoa(*( struct in_addr*)(host->h_addr_list[0])));
	printf("Scanning %s(%s)\n",al[1],inet_ntoa(*( struct in_addr*)(host->h_addr_list[0])));
        for(;port_iter < ac;port_iter++)
        {
		attack_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);	
		target.sin_port = htons(atoi(al[port_iter]));
		if((rc = connect(attack_sock,(struct sockaddr *)&target, sizeof(target))) == 0)
			printf("%15.15s %5.5s open\n",al[1],al[port_iter]);
		else
			printf("%15.15s %5.5s closed\n",al[1],al[port_iter]);
		close(attack_sock);
	}
	return 0;
}
