#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

void dump_names(FILE *);
char *normalize_name(const char *, char *, uint16_t);
int portable_getopt(int, const char **, int *, char *);

int
main(int ac, char **al) {
    FILE *fdin = NULL;
    if(ac == 2) {
        fdin = fopen(al[1], "r");
        if(!fdin) {
            printf("error opening file: %s\n", al[1]);
            return 1;
        }
        dump_names(fdin);
    } else {
        printf("usage: czu [lbr file]\n");
    }
    return 0;
}

void
dump_names(FILE *fdin) {
    unsigned char buf[128] = {0}, entry[33] = {0}, name[9] = {0}, ext[4] = {0};
    unsigned char n_name[9] = {0}, n_ext[4] = {0};
    uint8_t first = 0, max_size = 0;
    uint16_t m_idx = 0, m_length = 0;
    uint32_t idx = 0, cnt = 0, sze = 0, offset = 1;
    uint64_t pos = 0;
   
    /* there's actually *two* files packed into each 128 byte
     * block: 16 bytes for the file info, 16 bytes for the time
     * info, and then three more files.
     *
     * that also explains why the directory entry for pli*.lbr
     * says 33 blocks: there's 33 blocks, each with four files
     * in it. that's why you have to test if the first byte of
     * each 32 byte block is 0x00, because things might be
     * unused or the like.
     */
    while(!feof(fdin)) {
        /* set offset to 0, we use it in the loop below */
        offset = 0;

        /* read an actual sector of the file, and report
         * the offset below
         */
        if(fread(buf, sizeof(char), 128, fdin) < 128) {
            printf("error in read");
            break;
        }

        //printf("$$DEBUG: pos: %ld\n", ftell(fdin));
        for(int idx = 0; idx < 4; idx++ ) {
            //printf("$$DEBUG: offset: %d\n", offset);
            /* slice a sector up into entries */
            for(cnt = 0; cnt < 32; cnt++) {
                entry[cnt] = buf[offset + cnt];
            }
            entry[32] = '\0';
            offset += 32;
            for(cnt = 0; cnt < 8; cnt++) {
                name[cnt] = entry[1 + cnt]; 
            }
            name[8] = '\0';
            for(cnt = 0; cnt < 3; cnt++) {
                ext[cnt] = entry[9 + cnt];
            }
            ext[3] = '\0';
            m_idx = entry[13] + entry[12];
            m_length = entry[15] + entry[14];
            sze = m_length * 128;
            normalize_name(name, n_name, 9);
            normalize_name(ext, n_ext, 4);
            if(!first) {
                max_size = m_length;
                first = 1;
            }
            if(entry[0] == 0) {
                printf("entry: \"%s.%s\" (\"%s.%s\") %hu %hu %d\n", name, ext, n_name, n_ext, m_idx, m_length, sze);
            }
        }
        //printf("$$DEBUG: idx: %d\n", idx);
        if(idx >= max_size) {
            break;
        } else {
            idx++;
        }
    }
}

char *
normalize_name(const char *src, char *dst, uint16_t len) {
    uint16_t idx = 0, i_len = len - 1;
    // remember, names are 8.3, so we know we either have
    // a buffer of 8 or a buffer of 3, and spaces mean we
    // hit the end.
    for(; idx < i_len; idx++) {
        if(src[idx] == ' ') {
            break;
        }
        dst[idx] = tolower(src[idx]);
    }
    // we reserve the last spot for ASCII NUL
    dst[idx] = '\0';
}

int
portable_getopt(int ac, const char **al, const char *fmt, int *offset, char *optarg) {
    int start = *offset;
    if(start > offset) {
        return -1;
    }

    if(al[offset][0] == '-' && al[offset][1] != '\0') {

    }
}
