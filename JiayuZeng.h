/*	Header file for Hash table. Contains
	structural definitions and prototypes for hash table.
	   Created by: Yanli Zhu
 	   Edited  by: Jiayu Zeng
	   Date      : 11/20
*/
#ifndef teamProject_hashTable_h
#define teamProject_hashTable_h

#include <stdio.h>
#include <stdlib.h>


typedef struct collision
{
    void    *proPtr;
    struct  collision *next;
    struct  collision *pre;

}COLLNODE;

typedef struct
{
    void*   collPtr; // point to collnode
    int     counter;

} HASH;


    HASH*   Hash_Create	   (HASH *hash, int size);
    void    Hash_Insert	   (HASH *table, void *dataPtr, int locn);
    void    Hash_delete	   (HASH *table, char *target,  int locn,
						  int ( *compareCode) (void* argu1, void *argu2));


    void*   Hash_search	   (HASH *table, char *target,  int locn,
						  int ( *compareCode) (void* argu1, void *argu2));
    void    hash_Traverse   (HASH *table, int size, void (*process)(void *dataPtr));

    HASH*	  Hash_destory	   (HASH *table, int size);
    void    Hash_outputData (HASH *table, int size, FILE *fpOut,
						  void (write)(void *outData,void *filePtr));

    void    self_adjusted   (HASH *table, int locn,
						  int (*compareSearch)(void *arg1,void *arg2));

    void    self_adjusted2  (HASH *table, int size,
						  int (*compareSearch)(void *arg1,void *arg2));

    void    Hash_outputSearch	  (HASH *table, int size, FILE *fpOut, void *emptyPtr,
							 void (write)(void *outData,void *filePtr),
							 int (*compareSearch)(void *arg1, void *arg2));

    int     hash_Unique	   (HASH *table, int size, char *target,
						  int (*compareCode)(void *data1, void *data2));

#define     A_NUMNER        25
#define     B_NUMBER        34

#endif
