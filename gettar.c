/* a simple utility to fetch the contents of a 
 * tar file. Definitely based on the Plan9 idea
 * Copyright 2014 Stefan Edwards under zlib/png license
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
    FILE *fdin = nil, *fdout = nil;
    char buf[512] = {0}, size[10] = {0};
    uint32_t len = 0, idx = 0;
    uint8_t args_p = 0, print_p = 0;

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
            if(!feof(fdin)){
                printf("fread failed...\n");
            }
            return 3;
        }

        if(args_p){ // do we have args? if so, check if the filename is one of them
            for(idx = 2; idx < ac; idx++){
                // in tar-file format, the length of the file name is 100
                if(!strncmp(al[idx], buf, 100)){
                    print_p = 1;
                }
            }
        } else {
            print_p = 1;
        }

        if(print_p){
            if((fdout = fopen(buf, "w")) == nil){
                perror("fopen, failed!\n");
            }
        }
        // tar stores file size in packed octal ASCII
        // unpack it here; field size is 12
        for(idx = 124; idx < 137; idx++){
            if(buf[idx] < '0' || buf[idx] > '7')
                break;
            len = (len << 3) + buf[idx] - '0';
        }

        // since tar blocks are of 512 bytes, if
        // we have anything left over add 1
        // of course, don't do it willynilly, because
        // some files are stored with filesize == 0,
        // like directories
        if((len % 512) > 0){
            len /= 512;
            len += 1;
        } else {
            len /= 512;
        }

        // normally, we'd just fseek, but stdin may be processing
        // better to iteratively fread...
        for(idx = 0; idx < len; idx++){
            if((fread(buf, sizeof(char), 512, fdin)) < 512){
                printf("fread failed!\n");
                return 4;
            }
            if(print_p){
                fwrite(buf, sizeof(char), 512, fdout);
            }
        }

        fclose(fdout);
        print_p = 0;
    }
}
