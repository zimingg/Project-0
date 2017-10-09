
#include "mlist.h"
#include "mentry.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


static int N_add(const MList *ml, const MEntry *me);
static const MEntry *N_lookup(const MList *ml, const MEntry *me);
static void N_destroy(const MList *ml);
const MList *MList_create(void);




typedef struct nlist{
   
    MEntry * data;
    struct nlist * next;

}N_list;

typedef struct bucket{
    int size;
    N_list * list;
    
    
}Bucket;


static int resize(const MList * ml, N_list *nlist ){
    int i;
    //printf(" resized %d   \n",i );
    
    //printf("resized! \n");
    //printf("%d ",i);
    int size = ((Bucket*)ml->self)->size;
    int new_size = size * 2;
    
    fprintf(stderr, "Change size from %d to %d. \n",size,new_size);
    
    N_list * new_nlist = (N_list * )malloc(sizeof(N_list) * new_size);
    if (new_nlist == NULL){
        return 0;
    }
    
   
    for(i = 0; i < new_size; i++){
        (new_nlist+i)->data = NULL;
        (new_nlist+i)->next = NULL;
    }
    
    //N_list * old_list = ((Bucket*)ml->self)->list;

    
    ((Bucket*)ml->self)->size = new_size;
    ((Bucket*)ml->self)->list = new_nlist;
    
    for (i = 0; i < size; i++){
        
        N_list * index =  nlist+i;
        
        if(index == NULL){
            
        }
        else{
            
            while(index->next!=NULL){
                ml->add(ml,(MEntry*)index->data);
                index = index->next;
            }
            ml->add(ml,(MEntry*)index->data);
        }
    }
    
    N_list *index;
    N_list * temp;
    
    for(i = 0; i < size; i++){
        index = (nlist + i)->next;
        if (index == NULL){
            
            
        }
        else{
            
            while(index->next!=NULL){
                temp = index;
                index = index->next;
                //printf("%d",temp->next == NULL);
                //(temp->data)->destroy((temp->data));
                free(temp);
                temp = NULL;
            }
            //(index->data)->destroy((index->data));
            free(index);
        }
        
    }
    
   
    free(nlist);
    nlist = NULL;
    
    return 1;
    

//    i++;
//    //printf(" resized %d   \n",i );
//
//    //printf("resized! \n");
//    //printf("%d ",i);
//    
//    
//    
//    int size = ((Bucket*)ml->self)->size;
//    int new_size = size * 2;
//    
//    fprintf(stderr, "Change size from %d to %d. \n",size,new_size);
//    
//    N_list * new_nlist = (N_list * )malloc(sizeof(N_list) * new_size);
//    
//    //    if (new_nlist == NULL){
//    //        return NULL;
//    //    }
//    int i;
//    for(i = 0; i < size; i++){
//        (new_nlist+i)->data = NULL;
//        (new_nlist+i)->next = NULL;
//    }
//    
//    //N_list * old_list = (N_list*)ml->self;
//    
//    ((Bucket*)ml->self)->size = new_size;
//    ((Bucket*)ml->self)->list = new_nlist;
//    
//    i = 0;
//    for (i = 0; i < size; i ++){
//        
//        N_list * index =  nlist+i;
//        
//        if((index->data)!=NULL){
//            // printf("%lu",(index->data)->hash((index->data),new_size));
//            ml->add(ml,(MEntry*)index->data);
//        }
//        
//        while((index->next)!=NULL){
//            
//            index = index->next;
//            ml->add(ml,index->data);
//            
//        }
//    }
//    

}



/*
 * MList - created a new mailing list; if unsuccessful, return NULL
 */
const MList *MList_create(void){
    
    int size = 101;
    
    
    MList * the_mlist = (MList *) malloc(sizeof(MList));
    if(the_mlist == NULL){
        return NULL;
    }
    
    Bucket * the_bucket = (Bucket *) malloc(sizeof(Bucket));
    if (the_bucket == NULL){
        
        free(the_mlist);
        the_mlist = NULL;
        return NULL;
        
    }
    
    N_list * the_nlist = (N_list *) malloc(sizeof(N_list) * size);
    
    if (the_nlist == NULL){
        free(the_mlist);
        the_mlist = NULL;
        free(the_bucket);
        the_bucket = NULL;
        return NULL;
    }
    int i;
    for(i = 0; i < size; i++){
        (the_nlist+i)->data = NULL;
        (the_nlist+i)->next = NULL;
    }
    
    
    the_nlist->data = NULL;
    the_nlist->next = NULL;
    the_bucket->size = size;
    the_bucket->list = the_nlist;
    the_mlist->self = the_bucket;
    the_mlist->add = N_add;
    the_mlist->destroy = N_destroy;
    the_mlist->lookup = N_lookup;
    
    return the_mlist;
    
    
}

/*
 * now declare dispatch table structure
 */
    /*
     * the private data for the MList instance
     */

    
    /*
     * add - adds a new MEntry to the list;
     * returns 1 if successful, 0 if error (malloc)
     * returns 1 if it is a duplicate
     */
static int N_add(const MList *ml, const MEntry *me){
    
    Bucket * bucket =  ((Bucket*)ml->self);
    N_list *nlist = bucket->list;
    int counter = 0;
    int size = ((Bucket *)ml->self)->size;
    //printf("! %d \n",size);
    //printf("%d",me->);
    
    
    unsigned long hash_num = (me->hash(me, size));
    //printf("after return %d \n",hash_num);
    
    //int hash_num = 1;
    if (ml->lookup(ml,me) == NULL){
        // mentry not existed
        N_list *index = nlist+hash_num;
        
        if(index->data == NULL){
            index->data = me;
            return 1;
        }
        else{
            //handle
            
            
            
            while(index->next!=NULL){
                counter++;
                index = index->next;
            }
            
            N_list * new = (N_list *)malloc(sizeof(N_list));
            if (new == NULL ){
                return 0;
            }
            new->data = me;
            new->next = NULL;
            
            index->next = new;
            if(counter > 20){
                
                
                //resize
            int feedback = resize(ml, nlist);
                
            if (feedback == 0){
                    
                fprintf(stderr, "Resize failed!");
                    
                }

                
            }
    
            return 1;
        
        }

    }
    else{
        // mentry existed
        return 1;
        
        
        
    }
    
    return 0;
}


    /*
     * lookup - looks for MEntry in the list, returns matching entry or NULL
     */
static const MEntry * N_lookup(const MList *ml, const MEntry *me){
    
    Bucket * bucket =  ((Bucket*)ml->self);
    N_list *nlist = bucket->list;
    //printf("%s",((Address*)me->self)->surname);
    unsigned long hash_num = me->hash(me, ((Bucket *)ml->self)->size);
    //printf("after return %d \n",hash_num);
    //printf("lookup \n");
    
    N_list * index = nlist + hash_num;
    
    MEntry * a = index->data;
    if (a == NULL){return NULL;} //check
    
    if(me->compare(me, a)==0){
        return a;
    }
    
    while((index->next)!=NULL){
        
        index = index->next;
        a = index->data;
        //printf(a);
        
        if (a == NULL){return NULL;} //check

        if(me->compare(me, a)==0){
            return a;
        }
    }

    return NULL;
}



    /*
     * destroy - destroy the mailing list
     */

static void N_destroy(const MList *ml){
//    N_list * the_nlist = ((Bucket*)ml->self)->list;
//    int size = ((Bucket*)ml->self)->size;
//    int i;
//    N_list *index;
//    for(i = 0; i < size; i++){
//        index = the_nlist + i;
//        
//        while(index!=NULL){
//            N_list * temp = index;
//            index = index->next;
//            free(temp);
//            temp = NULL;
//        }
//
//    }
//    
//    
//   
//    Bucket * the_bucket = (Bucket*)ml->self;
//    free(the_bucket);
//    the_bucket = NULL;
//    free((void *)ml);
    
    
    N_list * the_nlist = ((Bucket*)ml->self)->list;
    int size = ((Bucket*)ml->self)->size;
    int i;
    N_list *index;
    N_list * temp;
    
    
    for(i = 0; i < size; i++){
        index = (the_nlist + i)->next;
        if (index == NULL){
            
            
        }
        else{
            
            while(index->next!=NULL){
                temp = index;
                index = index->next;
                //printf("%d",temp->next == NULL);
                (temp->data)->destroy((temp->data));
                free(temp);
                temp = NULL;
            }
            (index->data)->destroy((index->data));
            free(index);
        }
        
    }
    int l;
    for(l=0; l<size;l++){
        if((the_nlist+l)->data!=NULL){
            //free((the_nlist+i)->data);
            (the_nlist+l)->data->destroy((the_nlist+l)->data);
        }
    }
    free(the_nlist);
    // }
    
    
    
    
    Bucket * the_bucket = (Bucket*)ml->self;
    free(the_bucket);
    the_bucket = NULL;
    free((void *)ml);

    
}












