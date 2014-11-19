/*
    This file contains the definitons of the functions to maintain
	and process a hash table:

   Functions:
		Hash_Create
		Hash_destroy
		Hash_Insert
		Hash_delete
		hash_Traverse
		hash_unique
		Hash_outputData
		Hash_outputSearch

*/
#include "JiayuZeng.h"

/*	================= Hash_Create ================
	Initializes the hash table
		Pre:    hash - a pointer to HASH.
			   size - the size of hash table
		Post:   array of HASH is initialized
 
    Created by Yanli Zhu

 */
HASH* Hash_Create(HASH *hash, int size)
{
    // Local Definitions
    int i;

    // Statements
    hash = (HASH*) malloc (size * sizeof (HASH));
    if (hash)
    {
	   for (i = 0; i < size; i++)
        {
            (hash + i)->collPtr    = NULL;
            (hash + i)->counter   = 0;
        }
    }
    
    return hash;
}//create

/*====================== Hash_ Insert ===============================
	This function inserts data to hash table, if there is collison
	will insert data as a single link-list.

		pre : table   - a pointer to hash table (may be NULL)
			  dataPtr - pointer to a node containing valid data
			  locn    - index of hash table

		post: data inserted to hash table
    Created by Yanli Zhu
 */
void Hash_Insert(HASH *table, void *dataPtr, int locn)
{
    // Local Definitions
    COLLNODE *newNode = NULL;
    COLLNODE *collCur = (COLLNODE*)table[locn].collPtr;
    COLLNODE *collPre = NULL;

    // Statements

    newNode = (COLLNODE*)calloc(1, sizeof(COLLNODE));
    if(!newNode)
    {
        printf("allocate node fail\n");
        exit(104);
    }

    // initialize the new node
    newNode->proPtr	    = dataPtr;
    newNode->next	    = NULL;
    newNode->pre	    = NULL;
 
    if(table[locn].counter == 0) // add first node
        table[locn].collPtr = newNode;

    else  // collosion, use single linked list to add new collision node
    {
        while(collCur->next)
	   {
		  collPre = collCur;
		  collCur = collCur->next; // find end of linked list
	   }

        collCur->next   = newNode; // add new collision node to last->next
	   newNode->pre    = collCur;
    }
    table[locn].counter++;

    return;

}// Hash_insert

/*=================== Hash_destory ======================
	Destory the collision link-list.

		Pre:  table - a hash table contains valide data
			  size  - the hash table size
		Post: collision link-list got destroy.
			  table points to NULL
    ---Debugged by Yanli Zhu
 */
HASH* Hash_destory (HASH *table, int size)
{
    // Local Defination
    int i =0;
    COLLNODE *pPre = NULL;
    COLLNODE *pCur = NULL;
	
    // Statements
    for (i=0; i<size; i++)
    {
	   if(table[i].counter != 0)
	   {
		  pCur =  (COLLNODE*)table[i].collPtr;
		  while (pCur)
		  {
			 pPre = pCur;
			 pCur = pCur->next;
			 //free(pPre->proPtr);
			 free (pPre);// free the collision nodes

		  }// end while
	   }// end if and check next index

    }// end for
	
    free(table);
    //table = NULL;

    return NULL;

}// destory
/*=================== Hash_delete ======================
	Deletes a collision node if found

		Pre:   table   - a hash table contains valide data
			  target  - a string from user's input
			  locn    - location of hash table
			  compare - a pointer to function

		Post: if input barcode exists, the matched collison
			  node will be delete, and the data pointer will
			  be return. Else return NULL.
 */

void Hash_delete (	HASH *table, char *target,  int locn,
				int ( *compareCode) (void* argu1, void *argu2))
{
    // Local Defination
    COLLNODE *pCur =  (COLLNODE*)table[locn].collPtr;
    COLLNODE *pPre = NULL;
    
    //Statements
    if ( table[locn].counter == 0 )
	   return ;
    else
    {
	   while (pCur)// walk the linked-list
	   {
		  if (compareCode(pCur->proPtr, target))// pass data to a compare function
									    // if matches, delete the collision node
		  {
			  if (pCur->pre == NULL)					    // the first node
			  {
				  if (pCur->next)					    // if there is collision
				  {
					  pCur->next->pre      = NULL;	    // set second node has no pre-Node
				       table[locn].collPtr = pCur->next;    // set "pList" starts at second node
				  }
			  }// if not the fisrt

			  else if ( pCur->next == NULL)			    // the last node
			  {
				 pPre->next = NULL;					    // break the link to next node
				 pCur->pre  = NULL;

			  }// if not the last
			  else
			  {
				 pPre->next      = pCur->next;
				 pCur->next->pre = pPre;
				 pCur->next      = NULL;
				 pCur->pre       = NULL;

			  }
			  free (pCur); // free the collision node

		  	  pCur = NULL;
			  table[locn].counter--;
			  //return  temp; // return data pointer
		  }

		  else // if not found
		  {
			  pPre = pCur;
			  pCur = pCur->next;
		  }
	   }// end while ( end traverse the hash table)
    }// end else ( table[locn].counter != 0 )

    return ; // if hashtable has data, but none matches

}//

/*=================== Hash_search ======================
	Search hashtable by barcode. 

		Pre:	  table		- a hash table contains valide data
			  target		- a string from user's input
			  locn			- location of hash table
			  compareCode	- a pointer to function
							  which compares data

		Post: if input barcode exists, the matched structure will
			  be returned. Else return NULL.
 */
void* Hash_search ( HASH *table, char *target,  int locn,
				int ( *compareCode) (void* argu1, void *argu2))
{
    //Local Defination
    COLLNODE *pCur ;

    //Statements

    if (table[locn].counter == 0) // if nothing in the hash table
	   return NULL;
    else
    {
	   pCur = (COLLNODE*) table[locn].collPtr;
	   while (pCur)
	   {
		   if (compareCode (pCur->proPtr, target))// if match
			  return pCur->proPtr;
		   
		   else
			  pCur = pCur->next;
	   }// end while
    }

    return NULL;

}
/*=================== Hash_Traverse ======================
	Traverses the hashtable and print all the items

		Pre:  table   - a hash table contains valide data
			 size    - the hash table size
			 process - a pointer to function that prints
				      every data

		Post: data of hash table is printed.
 */
void hash_Traverse (HASH *table, int size, void (*process)(void *dataPtr))
{
    // Local Defination
    int i = 0;
    COLLNODE *collnode;

    // Statements
    for (i = 0; i < size; i++)// can not do table[i].collptr != NULL because the hash table is not full
    {
        if (table[i].counter != 0)
        {
            collnode = (COLLNODE* )table[i].collPtr;    // points to non-null element
            while(collnode)
            {
                process (collnode->proPtr);
                collnode = collnode->next;
            }
        }// end if

    }// end travesing table

    return ; 
}
/*=================== hash_Unique ======================
	Check if same unique key is existing.

		Pre:  table   - a hash table contains valide data
			 size    - the hash table size
			 char    - a pointer points to a string
					 input by a user
			 compareCode - a pointer to fucntion
					       comparing the code

		Post: Return 1 if the input barcode is unique
			 else return  0
 */

int hash_Unique (HASH *table, int size, char *target,
                 int (*compareCode)(void *data1, void *data2))
{
    // Local Defination
    COLLNODE *pCur ;
    int i = 0;

    // Statements

    for (i = 0; i <size; i++)
    {
	   pCur = (COLLNODE*) table[i].collPtr;
	   while (pCur)
	   {
		   if (compareCode(pCur->proPtr, target))// if match
			  return 0;// barcode is not unique
		   else
			  pCur = pCur->next;

	   }// end while
    }
    return 1;
}

/*=================== Hash_outputData ======================
	Writes rest of the items to file. 

		Pre:  table   - a hash table contains valide data
			 size    - the hash table size
			 fpOut   - a pointer points to a opened 
					 wrinting mode file
			 write   - a pointer to function that writes
				      data to file.

		Post: Writes all data to file.
*/
void Hash_outputData (HASH *table, int size, FILE *fpOut, 
				  void (write)(void *outData,void *filePtr))
{
    // Local Defination
    int i = 0;
    COLLNODE *collnode;

    // Statements
    for (i = 0; i < size; i++)// can not do table[i].collptr != NULL because the hash table is not full
    {
	   if (table[i].counter != 0)
        {
            collnode = (COLLNODE* )table[i].collPtr;
            while(collnode)
            {
                write (collnode->proPtr,fpOut);// write all the data to a file
                collnode = collnode->next;
            }
        }
    }
    return ;

}

/*=================== Hash_outputSearch ======================
	Writes the searched items to file. 

		Pre:  table   - a hash table contains valide data
			 size    - the hash table size
			 fpOut   - a pointer points to a opened 
					 wrinting mode file
			 emptyPtr- a pointer with 0 number of beings 
				      search
			 write   - a pointer to function that writes
				      data to file.
			 compareSearch - a pointer to fucntion
					       comparing the numOfSearch

		Post: Writes data to file if the data were 
			 searched by users.
 */
void Hash_outputSearch (HASH *table, int size, FILE *fpOut, void *emptyPtr,
                        void (write)(void *outData,void *filePtr),
                        int (*compareSearch)(void *arg1, void *arg2))
{
    // Local Defination
    
    int i = 0;
    COLLNODE *collnode;

    // Statements
    for (i = 0; i < size; i++)// can not do table[i].collptr != NULL because the hash table is not full
    {
	   if (table[i].counter != 0)
        {
            //printf("i = %d counter = %d\n", i, table[i].counter);
            collnode = (COLLNODE* )table[i].collPtr; // initializes the node
            while(collnode) // if the node is valid
            {
                if (compareSearch(collnode->proPtr, emptyPtr))
				write (collnode->proPtr,fpOut);// write to file if data was searched

                collnode = collnode->next;
            }
        }

    }// end for

    return ;
}// end outputSearched
