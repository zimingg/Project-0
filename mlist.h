#ifndef _MLIST_H_
#define _MLIST_H_

#include "mentry.h"

typedef struct mlist MList;	/* forward reference */

/*
 * MList - created a new mailing list; if unsuccessful, return NULL
 */
const MList *MList_create(void);

/*
 * now declare dispatch table structure
 */
struct mlist {
/*
 * the private data for the MList instance
 */
    void *self;

/*
 * add - adds a new MEntry to the list;
 * returns 1 if successful, 0 if error (malloc)
 * returns 1 if it is a duplicate
 */
    int (*add)(const MList *ml, const MEntry *me);

/*
 * lookup - looks for MEntry in the list, returns matching entry or NULL
 */
    const MEntry *(*lookup)(const MList *ml, const MEntry *me);

/*
 * destroy - destroy the mailing list
 */
    void (*destroy)(const MList *ml);
};

#endif /* _MLIST_H_ */
