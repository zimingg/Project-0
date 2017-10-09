#include "mentry.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static unsigned long N_hash(const MEntry *me, unsigned long size);
static void N_print(const MEntry *me, FILE *fd);
static int N_compare(const MEntry *me1, const MEntry *me2);
static void N_destroy(const MEntry *me);
const MEntry *MEntry_get(FILE *fd);


void append(char * a, const char b){
    
    int len = strlen(a);
    a[len] = b;
    a[len+1] = '\0';
    
}



typedef struct address {
    /*
     * the private data for an instance of MEntry
     */
    //void *self;
    char surname[30];
    char full_string[300];
    int house_number;
    int zip_number;
    
    
}Address;


/*
 * a constructor that obtains the next MEntry from `fd'
 *
 * returns NULL if end of file
 */
const MEntry* MEntry_get(FILE *fd){
    //printf("%s\n","time");
    //    if( feof(fd) ){
    //        return NULL;
    //    }
    
    MEntry *m_entry;
    Address *n_address;
    
    
    m_entry = (MEntry *) malloc (sizeof(MEntry));
    
    if (m_entry == NULL){
        free(m_entry);
        m_entry = NULL;
        return NULL;
    }
    
    
    //printf("%s\n","mef");
    //n_address = NULL;
    n_address = (Address *) malloc (sizeof(Address));
    if (n_address == NULL){
        free(n_address);
        n_address = NULL;
        free(m_entry);
        m_entry = NULL;
        return NULL;
    }
    
    
    strcpy(n_address->full_string,"");
    strcpy(n_address->surname,"");
    //printf("%s\n",n_address->full_string);
    //printf("%s\n","naf");
    char *split_word;
    
    
    char line[300];
    
    
    
    
    //first line
    if (fgets(line,50,fd)!=NULL){
        //printf("(%s)\n ", line);
        strcat(n_address->full_string,line);
        //printf("(%s)\n ", n_address->full_string);
        split_word = strtok(line," ,.\n");
        //printf("!!! %s ", split_word);
        
        if(split_word == NULL){
            
            free(n_address);
            n_address = NULL;
            free(m_entry);
            m_entry = NULL;
            return NULL;
        }

        
        int i;
        for(i = 0; split_word[i]; i++){
            split_word[i] = tolower(split_word[i]);
        }
        
        strcpy(n_address->surname, split_word);             //surname
    }
    else{
        
        free(n_address);
        n_address = NULL;
        free(m_entry);
        m_entry = NULL;
        return NULL;
    }
    
    
    
    //second line
    
    if (fgets(line,50,fd) !=NULL){
        //printf("(%s)\n ", line);
        strcat(n_address->full_string,line);
        
        split_word = strtok(line," ,.\n");
        //printf("!!! %s ", split_word);
        
        int num = atoi(split_word);
        if (num != 0){
            n_address->house_number = num;
            
        }
        else{
            
            n_address->house_number = 0;                  //house number
        }
        
        
    }
    else{
        free(n_address);
        n_address = NULL;
        free(m_entry);
        m_entry = NULL;
        return NULL;
    }
    //third line
    
    
    if(fgets(line,50,fd)!=NULL){
        //printf("(%s)\n ", line);
        strcat(n_address->full_string,line);               //full address
        
        split_word = strtok(line," ,.\n");
        
        
        char *list[20];
        int i = 0;
        while(split_word!=NULL){
            //printf("%s \n", split_word);
            list[i++] =  split_word;
            
            split_word = strtok(NULL, " ,.\n");
        }
        //printf("!!! %s ", list[i-1]);
        n_address->zip_number = atoi(list[i-1]);       //zip number
        
        
    }
    else{
        free(n_address);
        n_address = NULL;
        free(m_entry);
        m_entry = NULL;
        return NULL;
    }
    
    m_entry->self = n_address;
    m_entry->hash = N_hash;
    m_entry->compare = N_compare;
    m_entry->destroy = N_destroy;
    m_entry->print = N_print;
    
    
    return m_entry;
    
}






/*
 * computes a hash of the MEntry, mod `size'
 */
static unsigned long N_hash(const MEntry *me, unsigned long size){
    

    //printf(" <%s> ", ((Address*)me->self)->surname);
    char string[100]="";
    strcat(string,((Address*)me->self)->surname);
    char temp[20]="";
    sprintf(temp, "%d",((Address*)me->self)->zip_number );
    strcat(string,temp);
    sprintf(temp, "%d",((Address*)me->self)->house_number );
    strcat(string,temp);
    

    
    unsigned long hash = 5381;
    int c;
    char * buff = string;
    //printf(" !!%s \n",buff);
    
   
    
    while ((c = *buff++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    
    
    //printf("before return %lu \n",hash % size);
    strcpy(string,"");
    
    //printf(" <%d> ", hash % size);
    return hash % size;
    
    //return 0.0001L;
}

/*
 * prints the full address on `fd'
 */
static void N_print(const MEntry *me, FILE *fd){
    char str[300];
    strcpy(str, ((Address *)me->self)->full_string);
    fprintf(fd,"%s", str);
}

/*
 * compares two mail entries, returning <0, 0, >0 if
 * me1 < me2, me1 == me2, me1 > me2
 */
static int N_compare(const MEntry *me1, const MEntry *me2){
    //comapre surname
   
    int result = strcmp(((Address *) me1->self)->surname ,((Address *) me2->self)->surname);
   
    if (result > 0){
        return 1;
    }
    else if (result < 0){
        return -1;
    }
    
    //compare zip code
    result = ((Address *) me1->self)->zip_number - ((Address *) me2->self)->zip_number;
    
    if (result > 0){
        return 1;
    }
    else if (result < 0){
        return -1;
    }
    
    
    
    //compare house number
    
    result = ((Address *) me1->self)->house_number - ((Address *) me2->self)->house_number;
    if (result > 0){
        return 1;
    }
    else if (result < 0){
        return -1;
    }
    
    return 0;
    
}

/*
 * destroys the mail entry
 */
static void N_destroy(const MEntry *me){
    Address *nm = (Address *) me->self;
    //free(nm->surname);
    free(nm);
    free((void *)me);
    
}













