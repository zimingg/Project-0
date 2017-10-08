#include <stdio.h>
#include "mentry.h"
#include "mlist.h"

static void usage(char *program) {
    fprintf(stderr, "usage: %s [file]\n", program);
}

int main(int argc, char *argv[]) {
    const MEntry *mep, *meq;
    const MList *ml;
    FILE *fd;

    if (argc > 2) {
        usage(argv[0]); return -1;
    }
    if (argc > 1) {
        fd = fopen(argv[1], "r");
        if (fd == NULL) {
            fprintf(stderr, "Error opening %s\n", argv[1]);
            return -1;
        }
    } else
        fd = stdin;
        
    ml = MList_create();
    while ((mep = MEntry_get(fd)) != NULL) {
        
        meq = ml->lookup(ml, mep);
        if (meq == NULL)
            (void) ml->add(ml, mep);
        else {
            printf("Potential duplicate\n");
            printf("===================\n");
            mep->print(mep, stdout);
            printf("==========\n");
            mep->print(meq, stdout);
            printf("\n");
            mep->destroy(mep);
        }
    }
    ml->destroy(ml);
    if (fd != stdin)
        fclose(fd);
    return 0;
}
