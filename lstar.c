/* a simple utility to dump metadata information from a 
 * tar file. Wrote this a long time ago, in admiration
 * for Plan9 :D
 * Copyright 2011 Stefan Edwards under zlib/png license
 * see the LICENSE file for details
 */
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define nil NULL
#define nul '\0'

#define isprn(x) (x >= 33 && x <= 126) ? 1 : 0

int
main(int ac, char **al){
    FILE *fdin = nil;
    char buf[512] = {0}, size[10] = {0};
    uint32_t len = 0, idx = 0;
    uint8_t args_p = 0;

    if(ac == 1) {
        printf("Usage: lstar <tarfile> [<filename> ...]\nif <tarfile> is '-', then read from stdin\n");
        return 1;
    }
    else {
        if(!strncmp("-", al[1], 1)){
            fdin = stdin;
        } else {
            if((fdin = fopen(al[1], "r")) == nil){
                printf("Unable to open file \"%s\"\n", al[1]);
                return 2;
            }
        }

        if(ac > 2){
            args_p = 1;
        }
    }
    while(!feof(fdin)){
        if((fread(buf, sizeof(char), 512, fdin)) < 512){
            printf("fread failed...\n");
            return 3;
        }

        if(args_p){ // do we have args? if so, check if the filename is one of them

        } else {
            // filename is actually NUL terminated, so we can just print that buffer
            printf("%s\n", buf);
        }

        for(idx = 124; idx < 135; idx++){
            if(buf[idx] < '0' || buf[idx] > '7')
                break;
            len = (len << 3) + buf[idx] - '0';
        }
        // normally, we'd just fseek, but stdin may be processing
        // better to iteratively fread...
        len /= 512;
        len += 1;
        for(idx = 0; idx < len; idx++){
            if((fread(buf, sizeof(char), 512, fdin)) < 512){
                printf("fread failed!\n");
                return 4;
            }
        }
    }
}
