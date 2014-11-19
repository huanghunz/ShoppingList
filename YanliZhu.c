
/*	This program builds a shopping list with BST and HASH table,
    then searchs, inserts, deletes and prints and saves a shopping list.
	   Written by: Yanli Zhu
	   Date: 11/12/2012
*/

#include "YanliZhu.h"


int main (void)
{
//	Local Definitions

    char	   option = ' ';
    LISTHEAD*  list = NULL;

//	Statements
	
    welcome();
    
    list = listCreate();
    build(list);

    while ( (option = getOption ()) != 'Q')
    {
	   switch (option)
	   {
           case 'A': printList (list->pTree);
				  break;
           case 'B': printNameByTree(list->pTree);
				  break;
           case 'C': printHashTable (list->pHash, list->arysize);
                  break;
           case 'D': printCost(list->pTree);
                  break;
           case 'E': addData (list->pTree, list->pHash, list->arysize);
				  break;
           case 'F': searchByName(list->pTree,list->pHash,list->arysize);
				  break;
           case 'G': searchBarCode(list->pHash, list->arysize);
				  break;
           case 'H': deleteByName(list->pTree, list->pHash, list->arysize);
				  break;
           case 'I': deleteCode(list->pHash,list->arysize, list->pTree);
				  break;
           case 'J': printBestDeal(list->pTree);
                      break;
           case 'K': printEfficiency(list->pHash,list->arysize);
                      break;
           case 'S': saveToFile (list->pHash, list->arysize, 1);
				  break;

	   } // switch
    } // while


    saveToFile (list->pHash, list->arysize, 0); // added by Jiayu 12/04/2012

    list->pTree = BST_Destroy(list->pTree, freeName);

    list->pHash = Hash_destory(list->pHash, list->arysize);

    free(list);
    list = NULL;

    #ifdef _MSC_VER
        printf( _CrtDumpMemoryLeaks() ? "\n\nMemory Leak\n" : "\nNo Memory Leak!!!\n");
    #endif
	

    farewell();

    system("pause");
	return 0;
}	// main


/*==============================listCreate==============================
 Allocates dynamic memory for an listhead
 node and returns its address to caller
 Pre    none
 Post   head allocated or error returned
 Return head node pointer; null if overflow*/

LISTHEAD* listCreate(void)
{
    //	Local Definitions
    LISTHEAD *list = NULL;
    
    //	Statements
    
    // allocate memory for listhead
    list = (LISTHEAD*)malloc(sizeof(LISTHEAD));
    if (list)
    {
        //list->count = 0;
        list->arysize = 0;
        list->pHash = NULL;
        //only create the BST here because hash table need to read file to get hash size.
        list->pTree = BST_Create (compareProName);
    }
    else
    {
        printf("error allocate for list head!");
        exit(100);
    }
    
    return list;
}// listCreate


/*============================== build ==============================
	This function bulid BST tree and hash table by reading one source files.
		Pre: list passed by reference, table passed by reference
		Post: list with data, table with data
		Return: none
*/

void build(LISTHEAD* list)
{
//	Local Definitions
    FILE	  *fpIn;
    char	  buffer[100];
    PRODUCT *pro = NULL;
    int	  line = 0;
    //int hashSize = 0;
    int locn = 0;

//	Statements
    fpIn = fopen(INPUTFILE, "r");
    if(!fpIn)
    {
        printf("\nError reading %s\n", INPUTFILE);
        exit(101);
    }

    // count the lines in the input file.
    while(fgets(buffer, sizeof(buffer), fpIn))
        line++;

    //printf("\nInput file line = %d\n",line);


    //multiply the lines in the input file and choose the prime number
    list->arysize = getPrime(line*2);

    //printf("\nhash size  = %d\n",list->arysize);

    //initlialze the hash table
    list->pHash = Hash_Create(list->pHash, list->arysize);
    

    // move back fpIn to the beginning the file
    fseek(fpIn, 0, SEEK_SET);                                        
    
    while(fgets(buffer, sizeof(buffer), fpIn))
    {
        //printf ("buffer == %s", buffer );
        pro = getData(buffer);
        BST_Insert(list->pTree, pro);
        locn = keyToAddress(pro->barCode, list->arysize);

        Hash_Insert(list->pHash, pro, locn);
    }

    fclose(fpIn);

    return;
}//build


/*==============================getData==============================
	This function reads the buffer and stores the information into a
	PRODUCT structure.
		Pre:  The buffer string containing the information for
			  one product is passed to the function.
		Post: If valid, the student structure in the calling module is
			  filled with data read from the string
		Return: student with data
 */

PRODUCT*  getData (char buffer[])
{
 //	Local Defnitions
    int     scanRes = 0;
    char    tempName[MAXNAME];
    char    tempCode[MAXCODE];
    double  tempOriPrice = 0;
    double  tempSalePrice = 0;
    DATE    tempDate = {0};
    PRODUCT *product = NULL;

//	Statements
    scanRes = sscanf(buffer, "%s %[^;]%*c %lf %lf %d%*c %d%*c %d*c",
                     tempCode, tempName,
                     &tempOriPrice, &tempSalePrice,
                     &tempDate.month, &tempDate.day, &tempDate.year);


    if(scanRes != 7)
        printf("---Error reading product information!---- \n");
    else
    {
        //if read success, allocate PRODUCT NODE
        product = (PRODUCT*)calloc(1, sizeof(PRODUCT));

        if(!product)
        {
            printf("Not enough memory \n");
            exit(103);
        }
        //  if allocate NODE success, then assign temp data to student
       // printf ("tempCode: %s\n", tempCode );
        strcpy(product->barCode, tempCode);
        product->name           = allocateString( tempName );
        product->oriPrice	  = tempOriPrice;
        product->salePrice      = tempSalePrice;
        product->expire		  = tempDate ;
    }

    return product;
} // getData

/*==============================printMenu==============================
    This funciton print menu
 */

void printMenu (void)
{
//	Statements
    printf("\n ============================ MENU ============================\n");
    printf("Print:\tA   Print My Shopping List by name (alphabetical order)\n");
    printf("   \tB   Print My Shopping List by name (in tree form) \n");
    printf("   \tC   Print my shopping list randomly\n");
    printf("   \tD   Price subtotal and saving  \n");

    printf("Insert:\tE   Insert one product\n");

    printf("Search:\tF   Search one product by name\n");
    printf("   \tG   Search one product by barcode\n");

    printf("Delete:\tH   Delete product by name \n");
    printf("   \tI   Delete product by barcode \n");

    printf("Other:\tJ   Best Deal\n");
    printf("   \tS   Save Searched data to File\n");
    printf("   \tK   Check the Efficiency\n");
    printf("   \tQ   Quit\n");

    return;

}   // printMenu


/*==============================printHashTable==============================
	Prints a list of product.
		Pre:  list has been created (may be null)
		Post: products printed
*/
void printHashTable (HASH* table, int size)
{

//	Statements
	printListTitle();

	hash_Traverse (table, size, processProduct);

	printf("==============================End of Shopping List============================\n");
	return;
}	// printHashTable


/*	==============================getOption==============================
	Reads and validates processing option from keyboard.
	    Pre  nothing
	    Post valid option returned
*/
char getOption  (void)
{
//	Local Definitions
	char option;
	bool error;

    printMenu();

	do
	   {
	    printf("\nEnter Option: ");
	    scanf(" %c", &option);
           FLUSH;
	    option = toupper(option);
	    if   (  option == 'A' || option == 'B'
              || option == 'C' || option == 'D'
              || option == 'E' || option == 'F'
              || option == 'G' || option == 'H'
              || option == 'I' || option == 'J'
              || option == 'Q' || option == 'S'
              || option == 'K')
	          error = false;
	    else
	         {
	          printf("Invalid option. Please re-enter: ");
	          error = true;
	         } // if
	   } while (error == true);
	return option;
}	// getOption



/* ==============================allocateString ==============================
	Uses a statically allocated string to create a
	dynamically allocated string with the same contents.
		Pre:  tempString - statically allocated
		Post: string - dynamically allocated
 */
char *allocateString( char *tempString )
{
//	Local Declarations
    char *string;

//	Statements
    string = (char *)calloc(strlen(tempString) + 1, sizeof(char));
    if (string == NULL)
	{
        printf ("ERROR! Not enough memory to allocate string!\a\n");
		exit (103);
	}
    strcpy (string, tempString);
    return string;
}// allocateString

/*==============================printCost==============================
 This function print total cost and toal saving
 Pro: bstList with data
 Post: print total cost and toal saving
 return: none*/
void printCost(BST_TREE* list)
{

    CustomData  cost = {0};
    
    BST_TraverseInVar(list, (void*)&cost, processCalculate);
    
    printf("Your shopping cart:\n\tTotal Cost   :\t$%8.2f\n\tTotal Saving :  $%8.2f\n", cost.transaction.totalCost, cost.transaction.totalSavings);
    
    return;
}

/*==============================processCalculate ==============================
 calculate total cost and saving and print
 Pre:  pro is a pointer to a product
 Post: data printed and line advanced
 */
void processCalculate   (int level, void* dataPtr, void* pCustomData )
{
    //	Local Definitions
	PRODUCT pro = *((PRODUCT*) dataPtr);
    CustomData* pData = (CustomData*)pCustomData;
    
    //Statements
    
    pData->transaction.totalCost += pro.salePrice;
    pData->transaction.totalSavings += (pro.oriPrice - pro.salePrice);
    
   
    return;
}

/*==============================processCalDeal ==============================
 Calculate best deal 
 Pre:  pro is a pointer to a product
 Post: data printed and line advanced
 */
void processCalDeal   (int level, void* dataPtr, void* pCustomData )
{
    //	Local Definitions
	PRODUCT pro = *((PRODUCT*) dataPtr);
    CustomData* pData = (CustomData*)pCustomData;
    double deal = 0;

    //	Statements
    
    deal =  (pro.oriPrice - pro.salePrice) / pro.oriPrice ;
    if(deal >  pData->bestDeal.save )
    {
        pData->bestDeal.save = deal;
        pData->bestDeal.product = pro;
        
    }
    return;
}

/*==============================printBestDeal==============================
    This function print best deal
	   Pro: bstList with data
	   Post: print best deal
	   return: none*/
void printBestDeal(BST_TREE* list)
{
    // Local Defination
    CustomData  cost = {0};
    
    // Statements
    BST_TraverseInVar(list, (void*)&cost, processCalDeal);
    
    printf("Best Deal is:\n");
    processProduct(&(cost.bestDeal.product));
    printf("Save :%5.1f%%\n", cost.bestDeal.save * 100);
    
    return;
}


