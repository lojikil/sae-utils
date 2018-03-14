#include <stdio.h>

/* a microscopic version of the `strings` program.
 * iirc, the gruq had posted something on twitter 
 * regarding the contemporaneous GNU strings 
 * vulnerability, and had asked for shortest-possible
 * verisons of replacements that could be run on
 * potentially-malicious sources. This is one such
 * attempt: it's not featureful, doesn't really do 
 * much, but it prints roughly what you would expect
 * from a strings-alike command.
 */

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
