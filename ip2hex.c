#include <stdio.h>
#include <stdlib.h> /* for atoi */
#include <string.h> /* for strsep */

#define nil NULL

int
main(int ac, char **al)
{
    int hexip = 0;
    char *q[4] = {nil}, *t = nil;
    if(ac != 2)
    {
        printf("Usage: ip2hex address\n");
        return 1;
    }
    t = al[1];
    while(hexip < 4)
    {
        q[hexip] = strsep(&t,".");
        hexip++;
    }
    hexip = 0;
    hexip = ((atoi(q[0]) & 255) << 24) + 
            ((atoi(q[1]) & 255) << 16) +
            ((atoi(q[2]) & 255) << 8) +
            (atoi(q[3]) & 255);
    printf("%x\n",hexip);
    return 0;
}

