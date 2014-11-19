/* Contains functions related to hash table.
		Created by: Jiayu Zeng
		Created on: 11/15/2012

	Functions:
				keyToAddress
				getPrim

				deleteCode
				searchBarCode

				self_adjusted
				self_adjusted2

				writeTofile
				saveToFile

				compareNumSearch
				compareBarCode
				compareProName

				printEfficiency
				longestLink
				averageNode
				loadFactor

*/

#include "YanliZhu.h"

#include "JiayuZeng.h"

/*=================== keyToAddress ======================
	Searchs a barcode input by user.

		Pre:  table - a hash table contains valide data
			  size  - the hash table size
		Post: if input barcode exists, data of PRODUCT
			  will be printed. Else print a error message.
 */
int keyToAddress(char key[], int size)
{
	// Local Defination
  
    int sum = 0 , i = 0;

	// Statements
    
    while ( key[i] != '\0')// pick every  number
    {
        sum += (int)key[i];

        i++;
    }

    sum = A_NUMNER * abs(sum) + B_NUMBER;

    return sum % size;

}// keyToAddr
/*====================== getPrime ==========================
	This function get the closest prime number to
	as large as twice the data size
		pre:  size - twice large as original data size
		post: hash table size is returned.
*/
int getPrime (int size)
{
    //  Local Defination
    int i ;
    int k = 1, control = 0; // act as a flag for control checking prime

    int add = size , sub = size;

    //  Statement
    while (control == 0 ) // while not a prime number
    {
        for ( i = 2 ; i*i <= add ; i++)
            if ( add % i == 0 ) // if it's not a prime
                  k = 0;//

        if ( k != 0)
            control = 1; // get out of the while loop
        else
        {
            add++; // check a number( greater than the original) is prime or not
            k = 1;
        }

    }// end finding a prime larger than the size

    control = 0; // reset the flag
    while (control == 0 ) // while not a prime number
    {

        for ( i = 2 ; i*i <= sub ; i++)
            if ( sub % i == 0 ) // if it's not a prime
                  k = 0;//

        if ( k != 0)
            control = 1;
        else
	   {
            sub--; //check a number( smaller than the original) is prime or not
            k = 1;
        }

    }// end finding a prime smaller than the size

    if ( add - size > size - sub)// if add if fathur than sub
		return sub;
    else
        return add;

}   // getPrime


/*===================== deleteCode ======================
	Deletes PRODUCT by input a barcode.

		Pre:  table - a hash table contains valide data
			  size  - the hash table size
		Post: if input barcode exists, data of PRODUCT
			  will be deleted. Else print a error message.
 */

void deleteCode (HASH *table, int size, BST_TREE *list)
{
//  Local Defination

    PRODUCT	 *proPtr = NULL;
    int		 locn;
    char		 temp[MAXCODE];


//  Statements
    printf("Enter the barcode for deleting : ");
    scanf (" %s", temp);
    FLUSH;

    locn = keyToAddress(temp, size);

    proPtr = (PRODUCT*) Hash_search (table, temp, locn, compareBarcode);

    if( !proPtr)	// if there is no matched product
		printf("Product barcode %s does not exist.\n", temp);
	else
	{			// Hash_delete just doing cutting link
	    Hash_delete (table, temp, locn, compareBarcode);

				// BST_Delete is actully free all the allocted items
	    BST_Delete (list, proPtr, freeName);

	    printf("Product %s deleted! \n", temp); // and a success information

    }

    return;

} // deleteCode

/*=================== searchBarCode ======================
	Searchs a barcode input by user.

		Pre:  table - a hash table contains valide data
			  size  - the hash table size
		Post: if input barcode exists, data of PRODUCT
			  will be printed. Else print a error message.
 */
void searchBarCode(HASH *table, int size)
{
    //Local Defination
    char		 input[MAXCODE];
    int		 locn;
    PRODUCT	 *proPtr = NULL;

    //Statements
    printf("Enter the barcode you want to search: ");
    scanf (" %s",input); // need validation.
    FLUSH;

    locn = keyToAddress(input, size);

    if (!(proPtr = (PRODUCT*) Hash_search (table, input, locn, compareBarcode)))
        printf(" Product barcode  %s does not exist\n", input);
    // if product does not exist, print error message
    // else
    else
    {
	   processProductDet (proPtr);			   // print product's information

	   if (table[locn].counter > 1)		   // if hash[i] contains more than one nodes
		  self_adjusted (table, locn, compareNumSearch);
    }

    return;
}

/*=================== compareNumSearch ======================
	Comapres the number of being searched.

		Pre:  data1 - a void pointer points to valid data
			  data2 - a void pointer points to valid data
		Post: if data1's number is larger than data2's,
			  return 1, else return 0
 */
int compareNumSearch(void *data1, void *data2)
{
    // Local Defination
    PRODUCT ptr1 = *(PRODUCT*) data1;
    PRODUCT ptr2 = *(PRODUCT*) data2;

    // Statements
    if (ptr1.numOfSearch > ptr2.numOfSearch)// comparing the number of
								    // being searched
        return 1;

    //else
    return 0 ;
}

/*======================== self_adjusted ==============================
	Search hashtable by barcode. Print data if found, else
	print error message.

		Pre:   table		  - a hash table contains valide data
			   locn			  - location of hash table
			   compareSeacrch - a pointer of function that it will
								compare the number of search of data

		Post: the most search node will become the first node
*/

void self_adjusted (HASH *table, int locn,
                    int (*compareSearch)(void *arg1,void *arg2))
{
    // Local Defination
    COLLNODE *pList =   (COLLNODE*) table[locn].collPtr;
							 // set pointing to the first node
    COLLNODE *pPre	=   (COLLNODE*) table[locn].collPtr;
							 // points to the first node
    COLLNODE *pCur	=   pPre->next;
							 // points to the second node

    while ( pCur) // if second node is valid
    {
	   if (compareSearch(pCur->proPtr,pList->proPtr)) // let say a target node
	   {
		  pPre->next = pCur->next; //  original : 1 ---> 2 ----> 3 ---> 4 ---->null
							 // if the target node, for example node 2 was searched
							 // it becomes 1 --- 3 --- 4 ---> null

		  if (pCur->next != NULL)// if target is not the last node
							// (if its next node is not a null)
			 pCur->next->pre = pPre;
							 // then set the pre pointer points to the pre node
							 // before   3 ---> 2 ----> 1 --->null
							 // after	   3 --> 1---> null

		  // *****************************************************
		  // after connecting the pre node and next node
		  // I need to know where I should place the target node
		  // *****************************************************

		  if (compareSearch (pCur->proPtr, pList->proPtr))
					   // check if it's has a higher searched number than the
					   // first few nodes of the hash table.
		  {
			 pCur->next = pList;// pList maybe pointing to the first node
							// or second or third, those nodes inserted before
							// target node

			 if (pList->pre!= NULL)
				pList->pre->next = pCur;

			 else// if pList points to the first node of the table
			 {
				// since I don't set up a dummy node, I have to use the
				// begin pointer
				table[locn].collPtr = pCur;
				pCur->pre			= NULL;
			 }

			 pList->pre  = pCur;

            return;// finish ajust

		  }// end if
		  else
			 pList = pList->next; // walk the linked-list

	   }//end if ( if the searched number is greater than the pre node

	   pPre = pCur;
	   pCur = pCur ->next;	  // walk the linked-list

    }// end while (end if the node is null)

    return ;

}

/*======================== self_adjusted2 ==============================
	If users search data by name, this function will be called.
	If same name data are searched, both data will be updated.

		Pre:  table			 - a hash table contains valide data
			  size			 - the size of the hash table
			  compareSeacrch - a pointer of function that it will
								compare the number of search of data

		Post: the most search node will become the first node
*/

void self_adjusted2 (HASH *table, int size,
                    int (*compareSearch)(void *arg1,void *arg2))
{
    // Local Defination
    int		 j = 0;
    COLLNODE	 *pList ;//= (COLLNODE*) table[locn].collPtr;
    COLLNODE	 *pPre ;//=  (COLLNODE*) table[locn].collPtr;
    COLLNODE	 *pCur ;//= pPre->next;


    for (j = 0; j< size; j++ ) // Compared to another adjust function
						 // this function will walk through the whole table
    {
        if (table[j].counter >1)// if this index does not has two node
						  // it will walk to next index
        {
		  pList	=   (COLLNODE*) table[j].collPtr;
		  pPre	=   (COLLNODE*) table[j].collPtr;
		  pCur	=   pPre->next;

		  while ( pCur) // if second node is valid
		  {
			 if (compareSearch(pCur->proPtr,pList->proPtr)) // let say a target node
			 {
				pPre->next = pCur->next; //  original : 1 ---> 2 ----> 3 ---> 4 ---->null
							 // if the target node, for example node 2 was searched
							 // it becomes 1 --- 3 --- 4 ---> null

				if (pCur->next != NULL)// if target is not the last node
							// (if its next node is not a null)
				    pCur->next->pre = pPre;
							 // then set the pre pointer points to the pre node
							 // before   3 ---> 2 ----> 1 --->null
							 // after	   3 --> 1---> null

				if (compareSearch(pCur->proPtr,pList->proPtr))
					   // check if it's has a higher searched number than the
					   // first few nodes of the hash table.

				{
				    pCur->next = pList;// pList maybe pointing to the first node
							// or second or third, those nodes inserted before
							// target node

				    if (pList->pre!= NULL)
					   pList->pre->next = pCur;

				    else// if pList points to the first node of the table
				    {
					   // since I don't set up a dummy node, I have to use the
					   // begin pointer
					   table[j].collPtr	   = pCur;
					   pCur->pre		   = NULL;
				    }

				    pList->pre  = pCur;

				    return;// finish ajust
				}// end if

				else
				    pList = pList->next; // walk the linked-list

			 }//end if ( if the searched number is greater than the pre node

			 pPre = pCur;
			 pCur = pCur ->next;	  // walk the linked-list

		  }// end while (end if the node is null)
	   }// end if table[j].counter > 1)
    }// end : walk through the whole table

    return ;

}

/*=================== compareBarCode ======================
	Compares barcode from hash table and user's input.

		Pre:	 dataPtr - void pointer points to a structer.
				 target  - user's input barcode
		Post:	If input barcode exists, return 1.
				Else return 0.
 */
int compareBarcode (void *dataPtr, void *target)
{
	//  Local Defination
    PRODUCT pro = *((PRODUCT*) dataPtr);

	//  Statements
    if (strcmp (target, pro.barCode) == 0)
    {
	   return 1;
    }

	return 0;

}	// compareBarCode

/*=================== saveToFile ======================
	Save data to file.

		Pre:  table		- a hash table contains valide data
			  size		- size of hash table
			  control   - a control to choose function
						  to output data.

		Post:  (chosen) data is saved.
 */
void saveToFile (HASH *table, int size, int control)
{
    // Local Defination
    FILE	  *fpOut;
    char	  outFile[MAXNAME];
    PRODUCT empty;

    // statements
    printf("Please enter the output file name <xxx.txt>: ");
    scanf (" %s", outFile);

    fpOut = fopen(outFile,"w");
    if (!fpOut)
    {
        printf("Error creating file."), exit(989);
    }

    if (control == 0)   //  save all
	   Hash_outputData(table, size, fpOut, writeTofile);// hash table, file , write function
    else			    //  save part of the files
    {
	   empty.numOfSearch = 0;
	   Hash_outputSearch(table, size, fpOut, &empty, writeTofile, compareNumSearch );
    }

    fclose(fpOut);

    printf("save to file %s finished!\n\n ", outFile);

    return ;
}

/*=================== writeTofile ======================
	Write data into a file.

		Pre:  dataPtr	- a void pointer points to
						  valid data
			  fpOut	- a pointer to file.

		Post:  write data to a file.
 */
void writeTofile(void *dataPtr, void *fpOut)
{
    // Local Defination
    PRODUCT prod = *(PRODUCT*)dataPtr;

    // Statements

    fprintf(fpOut, "%12s  %s;  \t%10.2f  %10.2f  %02d/%02d/%04d\n",
				prod.barCode, prod.name,
				prod.oriPrice, prod.salePrice,
				prod.expire.month, prod.expire.day, prod.expire.year);

    return ;
}// write

/*=================== loadFactor ======================
	Calculate and print the load factor of hash table.

		Pre:  table		- a hash table contains valide data
			  size		- size of hash table

		Post:  print the load factor.
 */

void loadFactor (HASH *table, int size)
{
    // Local Defination
    int i;
    double load = 0.0;

    // Statements
    for (i = 0; i < size; i++)
	   if (table[i].counter > 0 )
		  load++;

    printf ("Load Factor is %2.1f%%\n\n", (load / size )* 100 );

    return ;
}

/*=================== averageNode ======================
	Calculate and print the average nodes from hash table

		Pre:  table		- a hash table contains valide data
			  size		- size of hash table

		Post:  print the the number of average nodes.
 */
void averageNode (HASH *table, int size)
{
    // Local Defination
    int i = 0;
    double  j = 0.0, k = 0.0;

    // Statements
    for (i = 0 ; i < size; i++)
    {
	   if (table[i].counter>0)
	   {
		  j += table[i].counter;
		  k++;
	   }
    }

    printf("Total nodes:\t\t%.1f\n", j);
    printf("Collsion amount:\t%.1f\n",k);
    printf("average of nodes is: \t%.2lf \n\n",  j/k);

    return ;

}

/*=================== longestLink ======================
	Search the longest linked-list from hash table

		Pre:  table		- a hash table contains valide data
			  size		- size of hash table

		Post:  print the longest linked-list.
 */
void longestLink(HASH *table, int size)
{
    // Local Defination
    int i = 0;
    int hold = 0;

    // Statements
    for (i=0; i<size; i++)
    {
	   if (table[i].counter > hold)
		  hold = table[i].counter;	    // get the longest one
    }

    printf("The longest linked-list has %d nodes\n\n", hold);

    return;
}



/*=================== welcome ======================
 Farewell.
	  Pre:	  nothing
	  Post:	  print the welcome sentences.
 */
void welcome(void)
{
	// Statements

	printf("\nWelcome to My Shopping List! \n");
	printf("You can search information for on sale products from the list,\n");
	printf("add, delete items if you like.\nAnd you can save the searched items to a file.\n");
	printf("Let's start! \n");

	printf("\nBegin Shopping List\n");

	return;
}
/*=================== farewell ======================
    Farewell.
	   Pre:	  nothing
	   Post:  print the farewell.
 */
void farewell(void)
{
	// Statements
	printf("\nEnd Shopping List\n");
	printf("\nList information is saved in your folder. \nThank you for using our product.\n\n");
	return;
}
/*	====================== compareProName ======================
    Compare two product name's and return low, equal, high.
	   Pre:  pro1 - a valid pointer points to a product
		    pro2 - a valid pointer points to a product

	   Post: return low (-1), equal (0), or high (+1)
 */
int compareProName (void* pro1, void* pro2)
{
    //	Local Definitions
    PRODUCT p1 = *((PRODUCT*)pro1);
    PRODUCT p2 = *((PRODUCT*)pro2);;

    //	Statements
	if ( strcmp( p1.name, p2.name) < 0)
        return -1;

	if ( strcmp( p1.name, p2.name) == 0)
        return 0;

	return +1;
}	// compareProName


/*=============== printEfficiency ===================================
    This fucntion print efficiency of hash table
        Pre:  table		- a hash table contains valide data
			  size		- size of hash table

		Post:  print the efficiency information
 */
void printEfficiency(HASH *table, int size)
{

    loadFactor(table, size);
    longestLink(table, size);
    averageNode(table, size);
    return;
}

