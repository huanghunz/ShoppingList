/* This file mainly calls BST functions and other functions 
		Created by: Wai Sin Law (Stephanie)
		Created on: 11/15/2012

	  //Includes functions for 
		//For print function A, B, C, D
			printListTitle	 
			printList		
			processProduct	 
			printNameByTree
			processTree	

		//For insert function E
			addData

		//For search function F, G
			searchByName
			processProductDet

		//For delete function H		
			deleteByName
			freeName

			//printNameByTree not necessary
			//insert = no memory leak??
*/

#include "YanliZhu.h"



/*	==================== printNameByTree ======================
	Calls the BST function to print the indented tree.
		Pre:  list - BST tree
		Post: indented tree printed
*/
void printNameByTree (BST_TREE* list)
{
//	Statements
	BST_TraverseInVar(list, NULL, processTree);
	return;
}	// printNameByTree

/*	==================== processTree ======================
	Prints a line of the shopping list with indented tree.
		Pre:  level - the level where the data is it
			  dataPtr - receives void* from BST function
			  pCustomData is not used here, it is put here in order
			  to reuse the BST traverseInVar function
		Post: indented tree printed
*/
void processTree (int level, void* dataPtr, void* pCustomData)
{
//	Local Declarations
	PRODUCT prod = *((PRODUCT*) dataPtr);
	int i;

//	Statements
	for (i = 0; i < level; i ++)
	       printf("      ");
	printf("|%d| %s \n", level, prod.name);
	return;
}	// processTree

/*	====================== addData ======================
	To insert a product into both BST and hash
	    Pre  BST, Hash table
			 size - hash size
	    Post product added (abort on memory overflow)
*/
void addData (BST_TREE* list, HASH *table, int size)
{
//	Local Definitions
	PRODUCT* proPtr;
	char tempCode[MAXCODE];
    char tempName[MAXNAME];
    int locn;
	int i;
	int success;

//	Statements
	strcpy(tempCode, "            \0");
	proPtr = (PRODUCT*)malloc (sizeof (PRODUCT));
	if (!proPtr)
	    printf("Memory Overflow in add\n"), exit(101);

	//read barcode
	//validation: must be 12-digit number, cannot enter same barcode
	do
	{
		success = 1;
		printf("Enter product barcode (12 digit):   ");
		scanf ("%s",  tempCode);
			FLUSH;

		for (i = 0; i < (MAXCODE-1); i++)
		{
			if (isdigit(tempCode[i]))
			{
				if ((tempCode[i] < '0') || (tempCode[i] > '9'))
					success = 0;
			}
			else success = 0;
		}

		if (strlen(tempCode) != (MAXCODE-1)) // 12-digit barcode
		    success = 0;

        if (! hash_Unique (table, size, tempCode, compareBarcode)) // check if barcode already exist
        {
            success = 0;
            printf("The barcode exists. Please try again.\n\n");
        }
		
		if (!success)
			printf("Wrong input! Please enter barcode again! \n\n"); // print error message
	}	while (!success);
	strcpy(proPtr->barCode, tempCode);
	
	//read product name
	printf("Enter product name: "); 
	scanf ("%59[^\n]", tempName);
		FLUSH;
    proPtr->name = allocateString(tempName);

	//read original price
	//validation: should be a double
	do
	{
		success = 0;
		printf("Enter product original price:  ");
		success = scanf ("%lf",  &(proPtr->oriPrice));
			FLUSH;
		if (!success)
			printf("Wrong input! Please enter the original price again! \n\n");
	}	while (!success);

	//read sale price
	//validation: should be a double, cannot >= original price
	do
	{
		success = 0;
		printf("Enter product sale price:  ");
		success = scanf ("%lf",  &(proPtr->salePrice));
			FLUSH;
		if ((proPtr->salePrice) >= (proPtr->oriPrice))
		{
			printf("It is not on sale! Please enter the sale price again! \n\n");
			success = 0;
		}
		if (!success)
			printf("Wrong input! Please enter the sale price again! \n\n");
	}	while (!success);

	//read end of sale date
	//validation: date, month, year should be int, check with the calendar system we used
	do
	{
		success = 0;
		printf("Enter end of sale date (mm/dd/yyyy):  ");
		success = scanf ("%d%*c%d%*c%d",  &(proPtr->expire.month),
						&(proPtr->expire.day),
						&(proPtr->expire.year) );
			FLUSH;

		if (((proPtr->expire.month) < 1) || ((proPtr->expire.month) > 12)) // check month
			success = 0;

		if (((proPtr->expire.year) < 1) || ((proPtr->expire.year) >= 9999)) // check year
			success = 0;
		 
		if (((proPtr->expire.month) == 1) || ((proPtr->expire.month) == 3) || // check day
			((proPtr->expire.month) == 5) || ((proPtr->expire.month) == 7) ||
			((proPtr->expire.month) == 8) || ((proPtr->expire.month) == 10)||
			((proPtr->expire.month) == 12))
		{
			if (((proPtr->expire.day) < 1) || ((proPtr->expire.day) > 31))
				success = 0;
		}
		else
		{
			if (((proPtr->expire.month) == 4) || ((proPtr->expire.month) == 6) ||
				((proPtr->expire.month) == 9) || ((proPtr->expire.month) == 11))
			{
				if (((proPtr->expire.day) < 1) || ((proPtr->expire.day) > 30))
					success = 0;
			}
			else
			{
				if ((proPtr->expire.year) % 4 == 0)  // when it is Feb
				{
					if (((proPtr->expire.day) < 1) || ((proPtr->expire.day) > 29))
					success = 0;
				}
				else
				{
					if (((proPtr->expire.day) < 1) || ((proPtr->expire.day) > 28))
					success = 0;
				}
			}
		}

		if (!success)
			printf("Wrong input! Please enter again! (mm/dd/yyyy) \n\n");
	}	while (!success);

	BST_Insert (list, proPtr);
	locn = keyToAddress(proPtr->barCode, size);
	Hash_Insert(table, proPtr, locn);

	proPtr = NULL;
	free(proPtr);
    return;
}	// addData

/*	==================== searchByName ======================
	It searches the name innput by user and output all matches.
		Pre:  BST, Hash table
			  size - hash size
		Post: product printed in detail form by calling processProductDet
			  and updated the self-adjucted linked list
 
    ----Debugged by Yanli Zhu
*/
void searchByName (BST_TREE* list, HASH *table, int size)
{
//	Local Declarations
	PRODUCT* prodPtr = NULL;
	char temp[MAXNAME];

//	Statments
	//user input
	printf("Enter product name: ");
	scanf ("%[^\n]", temp);	

	//allocate memory
     prodPtr = (PRODUCT*)calloc(1, sizeof(PRODUCT));
     if (!prodPtr)
     {
		printf("Not enough memory \n");
          exit(103);
     }

    // prodPtr->name = allocateString(temp);
    prodPtr->name = temp;

	//starts search
    if (BST_RetrieveAll(list, prodPtr, processProductDet))
        self_adjusted2(table, size, compareNumSearch);
    else
	   printf("Product \"%s\" not in file\n", temp);

	//free memory
	//free(prodPtr->name);
	free(prodPtr);
	return;
}	// searchByName

/*	==================== processProductDet ======================
	Prints the information of a product in detail form.
		Pre:  dataPtr - receives void* from BST function
		Post: numOfSearch increases by 1 as a result of search
			  products printed in detail form
 
*/
void processProductDet (void* dataPtr)
{
//	Local Declarations
	PRODUCT* prodPtr = (PRODUCT*) dataPtr;

//	Statements
	(prodPtr->numOfSearch)++;

	printf("\nProduct Information: \n");
	printf("Product Barcode: %s \n", prodPtr->barCode);
	printf("Product Name   : %s \n", prodPtr->name);
	printf("Original Price : %.2lf \n", prodPtr->oriPrice);
	printf("Sale Price     : %.2lf \n", prodPtr->salePrice);
	printf("Sale Until     : %d/%d/%d \n", prodPtr->expire.month, prodPtr->expire.day, prodPtr->expire.year);
     printf("\n");
	return;
}	// processProductDet

/*	====================== deleteByName ======================
	To delete all the matches found in the shopping list in BST 
	and hash table
	    Pre  BST, Hash table
			 hashSize - hash size
	    Post product(s) deleted
*/
void deleteByName (BST_TREE* list, HASH *table, int hashSize)
{
//	Local Declarations
	PRODUCT* prodPtr;
	PRODUCT* foundPtr;
	char temp[MAXNAME];
	char tempCode[MAXCODE];
	int locn;
	int count = 0;
	int flag = 1;

//	Statements
     strcpy(tempCode, "            \0"); 

    //user input
     printf("Please enter product name: ");
     scanf("%[^\n]", temp);
		FLUSH;

    //allocate memory
    prodPtr = (PRODUCT*)calloc(1, sizeof(PRODUCT));

    if (!prodPtr)
    {
		printf("Not enough memory \n");
        exit(103);
    }
	//prodPtr->name = allocateString(temp); 
    prodPtr->name = temp;

	//starts delete
	do
	{
		count++;

		//search for all matches
		foundPtr = (PRODUCT*) BST_Retrieve(list, prodPtr); 

		if (foundPtr == NULL)
		{
			flag = 0;
			if (count == 1) // Wrong input
				printf("Error! Product \"%s\" cannot be found \n", temp);
		}
		else
		{
			processProductDet(foundPtr); // print detail info of the product
			locn = keyToAddress(foundPtr->barCode, hashSize);
			Hash_delete(table, foundPtr->barCode, locn, compareBarcode); // delete in hash
			if (!BST_Delete(list, foundPtr, freeName)) // delete in BST
				printf("Error! Item \"%s\"cannot be deleted! \n", temp);
		} 
	} while (flag);
	printf("%d item(s) deleted \n", count-1);

	//free(prodPtr->name);
	free(prodPtr);
	return;
}	// deleteByName

/* ================== freeName ==================
    This function is to free memory in the structure's name.
	   Pre   dataPtr
	   Post  memory freed
*/
void freeName (void* dataPtr)
{
//	Local Declarations
	PRODUCT prod = *((PRODUCT*) dataPtr);

//	Statements
	free(prod.name);
	prod.name = NULL;
	return;
}	// freeName


/*	====================printListTitle	====================
 Created by Yanli Zhu
 Prints shoppint list title.
 Pre:  none
 Post: list title printed
 */
void printListTitle(void)
{
    printf("================================My Shopping List================================\n");
    printf("Barcode\t      Merchandise\t\t\t Original   Sale   Sale \n");
    printf(" \t         Name\t\t\t\t  Price     Price  Until\n");
    printf("--------------------------------------------------------------------------------\n");
    
    return;
}   // printListTitle

/*	====================printList	====================
 Created by Yanli Zhu
 Prints a list of products.
 Pre:  list has been created by BST(list may be null)
 Post: products printed by alphabetical order (in order traversal)
 */
void printList  (BST_TREE* list)
{
    //	Statements
    printListTitle();
	BST_TraverseIn (list, processProduct);
	printf("===============================End of Shopping List=============================\n");
	return;
}	// printList

/*	====================processProduct 	====================
 Created by Yanli Zhu
 Print data of a product in one line.
 Pre:  dataPtr - receives void* from BST function
 Post: data printed and line advanced
 */
void processProduct   (void* dataPtr)
{
    //	Local Definitions
	PRODUCT prod = *((PRODUCT*) dataPtr);
    
    //	Statements
	printf("%12s  %-35s$%6.2f  $%6.2f  %02d/%02d/%04d\n",
	       prod.barCode, prod.name,
           prod.oriPrice, prod.salePrice,
           prod.expire.month, prod.expire.day, prod.expire.year);
	return;
}	// processProduct
