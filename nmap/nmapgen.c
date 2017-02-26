#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define nil NULL
#define nul '\0'


int
main(int ac, char **al) {
    char *tag = nil, buffer[128] = {0};
    int base = 0, l = 0;
    FILE *fdin = nil;

    if(ac >= 3) {
        tag = al[2];
        fdin = fopen(al[1], "r");
        if(!fdin) {
            printf("unable to open file %s\n", al[1]);
        }
    }

    if(ac == 4) {
        base = atoi(al[3]);
    }

    while(1) {
        fgets(&buffer[0], 128, fdin);

        if(feof(fdin)) {
            break;
        }

        l = strnlen(buffer, 128);
        // remove the newline
        buffer[l - 1] = nul;
        printf("nmap -sS -PS -vvv -oA %s-tcp-%d %s | tee -a %s-tcp-%d.dump\n", tag, base, buffer, tag, base);
        printf("nmap -sU -PS -vvv -oA %s-udp-%d %s | tee -a %s-udp-%d.dump\n", tag, base, buffer, tag, base);
        base += 1;
    } 

    fclose(fdin);
    return 0;
}
