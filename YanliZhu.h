/*	Header file for Team Project, Group No.1. Contains
	structural definitions and prototypes.
	   Created by: Yanli Zhu
 	   Edited  by: Jiayu Zeng, Wai Sin Law
	   Created on: 11/15/2012
	   Last updated on: 12/6/2012
*/

#ifndef teamProject_list_h
#define teamProject_list_h


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#ifdef _MSC_VER
#include <crtdbg.h>
#endif

#include "bstADT.h"
#include "JiayuZeng.h"


#define FLUSH       while( getchar()!= '\n')
#define INPUTFILE   "shoppingL.txt"
#define MAXCODE 13
#define MAXNAME 60

typedef struct
{
    int day;
    int month;
    int year;
} DATE;

typedef struct
{
    char    barCode[MAXCODE];
    char    *name;
    double  oriPrice; //original price
    double  salePrice;
    DATE    expire;
    int     numOfSearch;

} PRODUCT;

typedef struct
{
    BST_TREE    *pTree;
    HASH        *pHash;
    int         arysize;

}LISTHEAD;


typedef struct
{
    double totalSavings;
    double totalCost;

} Transaction;
typedef struct
{
    double  save;
    PRODUCT product;
}BESTDEAL;

typedef union
{
    int            level;
    Transaction    transaction;
    BESTDEAL       bestDeal;
}CustomData;

//	Prototype Declarations

//in YanliZhu.c
LISTHEAD*	  listCreate		  (void);
void		  build               (LISTHEAD* list);
PRODUCT*	  getData			  (char buffer[]);
char		  getOption           (void);
char		  *allocateString	  (char *tempString );
void		  processCalculate	  (int level, void* dataPtr, void* pCustomData );
void		  printCost           (BST_TREE* list);
void		  printBestDeal       (BST_TREE* list);
void		  processCalDeal	  (int level, void* dataPtr, void* pCustomData );
void		  printHashTable	  (HASH* table, int size);

//in JiayuZeng.c
void		  welcome			  (void);
void		  farewell            (void);
int         keyToAddress		  (char key[], int size);
int		  getPrime		  (int size);
int		  compareBarcode	  (void *dataPtr, void *target);
int		  compareProName	  (void* pro1, void* pro2);
int		  compareNumSearch	  (void *data1, void *data2);
void		  loadFactor		  (HASH *hash, int size);
void		  longestLink		  (HASH *hash, int size);
void		  averageNode		  (HASH *hash, int size);
void		  saveToFile		  (HASH *table, int size, int control);
void		  writeTofile		  (void *dataPtr, void *fpOut);
void		  printEfficiency	  (HASH *table, int size);
void		  deleteCode		  (HASH *table, int size, BST_TREE *list);
void		  searchBarCode	  (HASH *table, int size);


//in WaiSinLaw.c
void		  printListTitle	  (void);
void		  printList		  (BST_TREE* list);
void		  processProduct	  (void* dataPtr);
void		  printNameByTree	  (BST_TREE * list);
void		  processTree		  (int level, void* dataPtr, void* pCustomData);
void		  addData			  (BST_TREE* list, HASH *table, int size);
void		  searchByName		  (BST_TREE* list, HASH *table, int size);
void		  processProductDet	  (void *dataPtr);
void		  deleteByName		  (BST_TREE* list, HASH *table, int hashSize);
void		  freeName		  (void* dataPtr);

#endif
