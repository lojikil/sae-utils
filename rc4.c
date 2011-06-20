/* my old implementation of RC4, that I had scribbled down from
 * a book whilst reading at the library, in 1999 or so.
 * Released under the zlib/png license.
 * Copy 2011 Stefan Edwards, under the zlib/png.
 * See the LICENSE file for details
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define swap(x,y) x^=y^=x^=y

int main(int ac, char **al)
{
        char s[256] = {0}, key[256] = {0};
        int i = 0, j = 0, keylength =0,c = 0;
        if(ac != 2)
        {
                printf("Usage: rc4 key\n");
                return 1;
        }
        /* ksa */
        strcpy(key,al[1]);
        keylength = strlen(key);
        for(;i < 256;i++)
                s[i] = i;
        //printf("key,length = %s,%d\n",key,keylength);
        for(i = 0; i < 256;i++)
        {
                j = (j + s[i] + key[i % keylength]) % 256;
                swap(s[i],s[j]);
        }
        //printf("beginning PRGA\n");   
        i = 0; j = 0;
        while(1)
        {
                scanf("%c",&c);
                if(feof(stdin))
                        break;
                i = (i + 1) & 0xFF;
                j = (j + s[i]) & 0xFF;
                swap(s[i],s[j]);
                printf("%c",c ^ s[((s[i] + s[j]) & 0xFF)]);
        }
        //printf("\n");
        return 0;
}
