#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sqlite3.h>

#define nil NULL
#define nul '\0'

static int res_callback(void *, int, char **, char **);
int nexttoken(char *, int, int);

int
main(int ac, char **al) {
    FILE *fdin = nil;
    sqlite3 *db = nil;
    char *errmsg = nil, *line = nil, *query = nil;
    char *owner = nil, *ttl = nil, *class = nil, *type = nil, *typedata = nil;
    int rc = 0, id = 0, status = 0, offset = 0, currecord = 0, curline = 0;
    size_t len = 0;

    /* misc setup: check how many args we have */
    if(ac != 3) {
        printf("Usage: dig2sqlite <dig file> <database>\n");
        return 1;
    }

    /* open the first argument as a file which we
     * will read dig records from.
     */
    fdin = fopen(al[1], "r");

    if(!fdin) {
        printf("Cannot open input file: %s\n", al[1]);
        status = 2;
        goto EXIT;
    }

    /* open up our SQLite database */ 
    rc = sqlite3_open(al[2], &db);

    if(rc) {
        printf("Cannot open sqlite file: %s\n", sqlite3_errmsg(db));
        status = 3;
        goto EXIT;
    }

    /* create our table */
    query = (char *) malloc(sizeof(char) * 1024); 
    if(!query) {
        printf("Malloc failed!");
        status = 4;
        goto EXIT;
    }
    snprintf(query, 1024, "CREATE TABLE digrecords (ID INT PRIMARY KEY, owner VARCHAR(255), ttl INT, class VARCHAR(2), recordtype VARCHAR(8), recorddata TEXT);");

    rc = sqlite3_exec(db, query, res_callback, 0, &errmsg);

    if(rc != SQLITE_OK) {
        printf("An error occurred whilst creating table: %s\n", errmsg);
        sqlite3_free(errmsg);
        status = 5;
        goto EXIT;
    } 

    /* and now for magic. We have to read our dig records, line by line
     * parse them, and stick them into the SQLite database.
     * we don't _actually_ split things apart. What we do instead if
     * - read in a line
     * - walk that line, eating white space
     * - set the members to pointers within the buffer.
     * so for example:
     * foobar. 10 IN A 127.0.0.1\n
     * owner will point to &buffer[0],
     * ttl will point to &buffer[9],
     * &c. We also have to modify the buffer
     * whilst parsing, so that it looks like this
     * when done:
     * foobar.\010\0IN\0A\0127.0.0.1\0
     * it's faster, we keep memory allocation to a minimum,
     * and we should be golden.
     */
    line = (char *)malloc(sizeof(char) * 512);

    if(!line) {
        printf("Malloc failed!");
        status = 6;
        goto EXIT;
    }

    while(1) {

        /* read our line from our input source */
        line = fgets(line, 512, fdin);

        if(!line) {
            break;
        }
        
        len = strnlen(line, 512);
        curline += 1;

        /* if the line contains data AND isn't a comment...*/
        if(len != 1 && line[0] != ';') {
            /* slice up the line */
            owner = &line[0];
            offset = nexttoken(line, offset, len);
            ttl = &line[offset];
            offset = nexttoken(line, offset, len);
            class = &line[offset];
            offset = nexttoken(line, offset, len);
            type = &line[offset];
            offset = nexttoken(line, offset, len);
            typedata = &line[offset];

            /* kill the newline */
            line[len - 1] = '\0';

            /* start our parsing over again */
            offset = 0;

            /* generate our SQL query */
            snprintf(query, 1024, "INSERT INTO digrecords (id, owner, ttl, class, recordtype, recorddata) VALUES (%d, '%s', %s, '%s', '%s', '%s');",
                     currecord,
                     owner,
                     ttl,
                     class,
                     type,
                     typedata);

            currecord += 1;

            /* run it */
            rc = sqlite3_exec(db, query, res_callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                printf("An error occurred inserting records: %s, line: %d\n", errmsg, curline);
                sqlite3_free(errmsg);
            }
        }
    }

EXIT:
    sqlite3_close(db);
    fclose(fdin);
    free(query);
    free(line);
    return status;
}

static int
res_callback(void *data, int argcount, char **args, char **colnames) {
    int idx = 0;
    for(; idx < argcount; idx ++) {
        printf("%s = %s\n", colnames[idx], args[idx] ? args[idx] : "NULL");
    }
    return 0;
}

int
nexttoken(char *line, int offset, int len) {
    int flag = 0;

    if(line == nil || offset > len) {
        return -1;
    }

    for(; offset < len; offset++) {
        if(line[offset] == ' ' || line[offset] == '\t') {
            line[offset] = '\0';
            flag = 1;
        } else if(line[offset] != ' ' && line[offset] != '\t' && flag) {
            break;
        }
    }
    return offset;
}
