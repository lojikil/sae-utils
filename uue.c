/* A very simple UUEncode program that 
 * reads from stdin & filters to stdout.
 * Works on *nix & Microsoft.
 * Copyright 2004-2011 Stefan Edwards,
 * released under the zlib/png license.
 */
#ifdef _MSC_VER
#include "stdafx.h"
#endif
#include <stdio.h>

int
main(int ac, char **al)
{
        char ob[61] = {0}, ib[45] = {0};
		int i0 = 0, i1 = 0, mon = 0, op = 0;
        if(ac != 3)
        {
                fprintf(stderr,"Usage: %s remote_file mode < input > output\n",al[0]);
                return 2;
        }
        printf("begin %s %s\n",al[2],al[1]);
        while((mon = fread(ib,sizeof(char),45,stdin)) > 0)
        {
                if(mon < 45)
                {
                        for(i0 = mon;i0 < 45;i0++)
                                ib[i0] = 0;
                }
                for(i0 = 0,i1=0;i0 < 60;i0++)
                {
                        /* Let us make some nasty assumptions about
                         * word location and orientation shall we?
                         */
                        switch(i0 % 4)
                        {
                                case 0:
                                        ob[i0] = ((ib[i1] & 0xFC)  >> 2) + 32;
                                        break;
                                case 1:
                                        ob[i0] = (((ib[i1] & 0x03) << 4) + ((ib[i1 + 1] & 0xF0) >> 4)) + 32;
                                        i1++;
                                        break;
                                case 2:
                                        ob[i0] = (((ib[i1] & 0x0F) << 2) + ((ib[i1 + 1] & 0xC0) >> 6)) + 32;
                                        i1++;
                                        break;
                                case 3:
                                        ob[i0] = (ib[i1] & 0x3F) + 32;
                                        i1++;
                                        break;
                        }
                        ob[i0] = (ob[i0] != 32) ? ob[i0] : '`';
                }
                ob[60] = '\0';
                printf("%c%s\n",(mon + 32),ob);
        }
        printf("`\nend\n");
        //exits("normal");
		return 0;
}
