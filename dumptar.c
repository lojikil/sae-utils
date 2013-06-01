/* a simple utility to dump metadata information from a 
 * tar file. Wrote this a long time ago, in admiration
 * for Plan9 :D
 * Copyright 2011 Stefan Edwards under zlib/png license
 * see the LICENSE file for details
 */
#include <stdio.h>
#include <string.h>
#define isprn(x) (x >= 33 && x <= 126) ? 1 : 0
/* To Do: modify this so that it dumps the meta-info
 * of each file in the archive, instead of simply the
 * first file... =)
 */
int main(int ac,char **al)
{
	char file[512];
	int iter = 0, size = 0;
	FILE *fdin = NULL;
	if(ac == 1)
		fdin = stdin;
	else
	{
		if((fdin = fopen(al[1],"r")) == NULL)
		{
			printf("Cannot open tar file: %s\n",al[1]);
			return 2;
		}
	}
    while(1)
    {    
        if(feof(fdin))
            break;
        if(fread(file,sizeof(char),512,fdin) < 512)
        {
            fprintf(stderr,"Invalid tar archive!\n");
            return 1;
        }
        printf("dump tar meta-info\n");
        printf("filename: %s\n",file);
        printf("type flag: %i\n",file[156]);
        printf("file mode: ");
        for(iter = 157;iter < 257;iter++)
        {	
            if(!isprn(file[iter]))
                break;
            printf("%c",file[iter]);
        }
        printf("\nfile size: ");
        for(iter = 124; iter < 135; iter++)
        {
            if(file[iter] < '0' || file[iter] > '7')
                break;
            size = (size << 3) + file[iter] - '0';
        }
        printf("%i", size);
        printf("\nUSTAR magic token: ");		
        for(iter = 257;iter < 263;iter++)
        {
            if(!isprn(file[iter]))
                break;
            printf("%c",file[iter]);
        }
        printf("\nustar version: 0x%X 0x%X\n",file[263],file[264]);	
        printf("user name: ");
        for(iter = 265;iter < 297;iter++)
        {	
            if(!isprn(file[iter]))
                break;
            printf("%c",file[iter]);
        }
        printf("\ngroup name: ");
        for(iter = 297;iter < 329;iter++)
        {	
            if(!isprn(file[iter]))
                break;	
            printf("%c",file[iter]);
        }
        printf("\n");
        printf("offset: %d\n", ((size / 512) + 1) * 512);
        (void)fseek(fdin, ((size / 512) + 1) * 512, SEEK_CUR);
    }
	return 0;
}
