#ifndef _MENTRY_H_
#define _MENTRY_H_

#include <stdio.h>

typedef struct mentry MEntry;	/* forward reference */

/*
 * a constructor that obtains the next MEntry from `fd'
 *
 * returns NULL if end of file
 */
const MEntry *MEntry_get(FILE *fd);

/*
 * now define the dispatch table
 */
struct mentry {
/*
 * the private data for an instance of MEntry
 */
    void *self;
    
/*
 * computes a hash of the MEntry, mod `size'
 */
    unsigned long (*hash)(const MEntry *me, unsigned long size);
    
/*
 * prints the full address on `fd'
 */
    void (*print)(const MEntry *me, FILE *fd);
    
/*
 * compares two mail entries, returning <0, 0, >0 if
 * me1 < me2, me1 == me2, me1 > me2
 */
    int (*compare)(const MEntry *me1, const MEntry *me2);
    
/*
 * destroys the mail entry
 */
    void (*destroy)(const MEntry *me);
    
    
};

#endif /* _MENTRY_H_ */
