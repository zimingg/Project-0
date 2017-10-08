#include "mentry.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INITIAL_ARRAY_SIZE 10000

typedef struct data {
    long size;
    long len;
    const MEntry **entries;
} Data;

static int process_file(FILE *fd, Data *d) {
    const MEntry *me;

    while ((me = MEntry_get(fd)) != NULL) {
        if (d->len >= d->size) {    /* must resize */
            d->entries = realloc(d->entries, 2*d->size*sizeof(MEntry *));
            d->size *= 2;
        }
        d->entries[d->len++] = me;
    }
    return 1;
}

static int compare(const void *v1, const void *v2) {
    const MEntry *me1 = *(MEntry **)v1;
    const MEntry *me2 = *(MEntry **)v2;
    return me1->compare(me1, me2);
}

int main(int argc, char *argv[]) {
    Data d;
    long i;

    d.size = INITIAL_ARRAY_SIZE;
    d.len = 0;
    if ((d.entries = (const MEntry **)malloc(d.size * sizeof(MEntry))) == NULL) {
        fprintf(stderr, "Heap allocation failure\n");
        return -1;
    }
    if (argc == 1)
        process_file(stdin, &d);
    else {
        int i;
        for (i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-") == 0)
                process_file(stdin, &d);
            else {
                FILE *fd = fopen(argv[i], "r");
                if (fd != NULL) {
                    process_file(fd, &d);
                    fclose(fd);
                } else {
                    fprintf(stderr, "%s: unable to open file %s\n",
                            argv[0], argv[i]);
                }
            }
        }
    }
    qsort((void *)d.entries, d.len, sizeof(MEntry *), compare);
    for (i = 0; i < d.len; i++) {
        d.entries[i]->print(d.entries[i], stdout);
        d.entries[i]->destroy(d.entries[i]);
    }
    free(d.entries);
    return 0;
}
