/*@(#) ipcalc.c: a dead simple CIDR calculator.
 *@(#) usage: ipcalc [ipaddress mask]
 *@(#) if `ipaddress' and `mask' are missing, it defaults to a prompted version
 *@(#) copy right 2011 Stefan Edwards under the zlib/png license
 *@(#) see the LICENSE file for details
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define nil NULL
#define nul '\0'

int
main(int ac, char **al)
{
	char *buf = nil, *mask = nil, *sep = nil, *obuf = nil, *omask = nil;
	int address = 0, netmask = 0, offset = 0, data[] = {3,2,1,0}, wildcard = 0, iter = 0;
	buf = (char *)malloc(sizeof(char) * 17);
	obuf = buf;
	if(ac != 3)
	{
		mask = (char *)malloc(sizeof(char) * 3);
		omask = mask;
		printf("Enter address: ");
		fgets(buf,17,stdin);
		printf("Enter bitmask: ");
		fgets(mask,3,stdin);
		netmask = atoi(mask);
		if(netmask < 1 || netmask > 32)
		{
			printf("invalid bitmask\n");
			goto error_out;
		}
	}
	else
	{
		strncpy(buf,al[1],17);
		netmask = atoi(al[2]);
		if(netmask < 1 || netmask > 32)
		{
			printf("invalid bitmask\n");
			goto error_out;
		}
	}
	while(buf != nil)
	{
		sep = strsep(&buf,".");
		address += atoi(sep) << (data[offset] * 8);
		offset++;
	}
	printf("%x/%d:\n",address,netmask);
	for(;iter < (32 - netmask);iter++)
		wildcard |= (1 << iter);
	snprintf(obuf,17,"%d.%d.%d.%d",(wildcard & 0XFF000000) >> 24, (wildcard & 0x00FF0000) >> 16, (wildcard & 0x0000FF00) >> 8, wildcard & 0xFF);
	printf("wildcard: %s (%8.8x)\n",obuf,wildcard);
	printf("hosts: %d\n",(0xFFFFFFFF & wildcard) - 2);
	iter = (address & ~wildcard);
	snprintf(obuf,17,"%d.%d.%d.%d",(iter & 0xFF000000) >> 24, (iter & 0x00FF0000) >> 16, (iter & 0x0000FF00) >> 8, iter & 0xFF);
	printf("network: %s\n",obuf);
	iter += (0xFFFFFFFF & wildcard);
	snprintf(obuf,17,"%d.%d.%d.%d",(iter & 0xFF000000) >> 24, (iter & 0x00FF0000) >> 16, (iter & 0x0000FF00) >> 8, iter & 0xFF);
	printf("broadcast: %s\n",obuf);
error_out:
	free(obuf);
	if(ac != 3)
		free(omask);
	return 0;
}	
