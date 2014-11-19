/*	Header file for Binary Search Tree (BST). Contains
	structural definitions and prototypes for BST.
	   Written by: Wai Sin Law (Stephanie)
	   Created on: 11/15/2012
*/

typedef enum {false, true} bool;

//	BST type definitions
typedef struct node
	{
	 void*        dataPtr;
	 struct node* left;
	 struct node* right;
	} NODE;

typedef struct
	{
	 int   count;
	 int  (*compare) (void* argu1, void* argu2);
	 NODE*  root;
	} BST_TREE;

//	BST: Prototype Declarations for public functions
	BST_TREE* BST_Create	  (int (*compare) (void* argu1, void* argu2));
	BST_TREE* BST_Destroy	  (BST_TREE* tree, void (*freeN) (void* dataPtr));
	bool  BST_Insert		  (BST_TREE* tree, void* dataPtr);
	bool  BST_Delete		  (BST_TREE* tree, void* dltKey,
							 void (*freeN) (void* dataPtr));

	void* BST_Retrieve		  (BST_TREE* tree, void* keyPtr);
	bool  BST_RetrieveAll	  (BST_TREE* tree, void* dataPtr,
							 void (*process) (void* dataPtr));
	void  BST_TraverseIn	  (BST_TREE* tree,
							 void (*process)(void* dataPtr));
    void  BST_TraverseInVar	  (BST_TREE* tree, void* pCustomData,
							 void (*process) (int level, void* dataPtr, void* pCustomerData)); //WaiSin Law

	bool BST_Empty			  (BST_TREE* tree);
	bool BST_Full			  (BST_TREE* tree);
	int  BST_Count			  (BST_TREE* tree);