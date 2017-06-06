/* Tiny hex editor, minus the unix specific 'read' call.
 */
#include <stdio.h>

int
main()
{
    unsigned long flen = 0;
    unsigned char buf[16] = {0};
    int rdlen = 0, iter = 0;
    while((rdlen = fread(buf,sizeof(unsigned char),16,stdin)) > 0)
    {
        printf("%8.8lx: ",flen);
        for(iter = 0;iter < 16;iter++)
        {
            if(iter >= rdlen)
                printf("   ");
            else
                printf("%2.2x ",buf[iter]);
            if(buf[iter] < 33 || buf[iter] > 126)
                buf[iter] = '.';
        }
        printf(" ");
        for(iter = 0;iter < rdlen;iter++)
            printf("%c",buf[iter]);
        printf("\n");
        flen += 16;
    }
}
