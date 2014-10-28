#include <stdio.h>

int
main(int ac, char **al){
    FILE *f = NULL;
    int c = 0, i = 0;
    if(ac != 2){
        return 1;
    }
    if((f = fopen(al[1], "r")) == NULL){
        return 2;
    }
    while(!feof(f)){
        c = fgetc(f);
        if(c > 32 && c <= 127){
            printf("%c", c);
            i = 1;
        } else {
            if(i == 1){
                printf("\n");
                i = 0;
            }
        }
    }
    fclose(f);
}
