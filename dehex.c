#include <stdio.h>

int
main(int ac, char **al) {
    int i = 1, tmp = 0;
    for(; i < ac; i++) {
        sscanf(al[i], "%x", &tmp);
        printf("%c", (char) tmp & 255);
    }
    printf("\n");
}
