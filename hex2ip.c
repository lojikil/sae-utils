/* quick utility to convert a hex address to
 * dotted quads. See ip2hex for the opposite.
 * Copyright, 2011 Stefan Edwards, under the zlib/png license
 * see the LICENSE file for details
 */
#include <stdio.h>
int
main(int ac, char **al)
{
	unsigned int hexaddr = 0;
	if(ac != 2)
	{
		printf("Usage: hex2ip [hex ip address]\n");
		return 1;
	}
	sscanf(al[1],"%x",&hexaddr);	
	printf("%d.%d.%d.%d\n",
           (hexaddr & 0xFF000000) >> 24,
           (hexaddr & 0x00FF0000) >> 16,
           (hexaddr & 0x0000FF00) >> 8,
           (hexaddr & 0x000000FF ));
	return 0;
}
