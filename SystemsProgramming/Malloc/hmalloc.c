#include "hmalloc.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
/*You may include any other relevant headers here.*/


/*Add additional data structures and globals here as needed.*/
void *free_list = NULL;

/* traverse
 * Start at the free list head, visit and print the length of each
 * area in the free pool. Each entry should be printed on a new line.
 */
void traverse(){
   /* Printing format:
	 * "Index: %d, Address: %08x, Length: %d\n"
	 *    -Index is the position in the free list for the current entry. 
	 *     0 for the head and so on
	 *    -Address is the pointer to the beginning of the area.
	 *    -Length is the length in bytes of the free area.
	 */
	int index = 0;
	int* freed = (int*) free_list;
	if(freed != NULL){   //check if there are elements in free list
		freed++;
		while(*freed != 0){   //print out each element in free list
			int dist = *freed;
			printf("Index: %d, Address: %p, Length: %d\n", index, (freed + 1), *(freed - 1));
			freed = freed + dist;
			index++;
		}   //print last element in free list
		printf("Index: %d, Address: %p, Length: %d\n", index, (freed + 1), *(freed - 1));
		freed--;
	}
}

/* hmalloc
 * Allocation implementation.
 *    -will not allocate an initial pool from the system and will not 
 *     maintain a bin structure.
 *    -permitted to extend the program break by as many as user 
 *     requested bytes (plus length information).
 *    -keeps a single free list which will be a linked list of 
 *     previously allocated and freed memory areas.
 *    -traverses the free list to see if there is a previously freed
 *     memory area that is at least as big as bytes_to_allocate. If
 *     there is one, it is removed from the free list and returned 
 *     to the user.
 */
void *hmalloc(int bytes_to_allocate){
	int* ptr = NULL;
	int* ptr2 = (int*) free_list;
	int* prevPtr = ptr2;
	if (ptr2 == NULL){   //free list is empty
		ptr = (int*) sbrk(0);
        	sbrk(bytes_to_allocate + 8);
	} else {   //free list is not empty
		while (*(ptr2 + 1) != 0){   //while there are still elements in free list
			if (*ptr2 >= bytes_to_allocate){   //if the size is large enough to allocate
				ptr = ptr2;
				if (ptr2 == free_list){   //if the space to be allocated is the head
					free_list = ptr2 + *(ptr2 + 1);   //set free list head to next element
				} else {   //not head or tail of free list
					prevPtr++;   //re-link the list addresses
					ptr2++;
					*prevPtr = *prevPtr + *ptr2;
				}
				break;   //since we found a space to reallocate
			} else {   //if the space is not large enough to allocate
				prevPtr = ptr2;   //move to next element
				ptr2 = ptr2 + *(ptr2 + 1);
			}
		}
		if (ptr == NULL){   //check if space has already been allocated (has to iterate over last free list element)
			if (*ptr2 >= bytes_to_allocate){   //check if size is large enough to allocate
                                ptr = ptr2;
				prevPtr++;
				*prevPtr = 0;
			} else {   //no space large enough was found, creating new area
				ptr = (int*) sbrk(0);
                		sbrk(bytes_to_allocate + 8);
			}
		}
	}
	*ptr = bytes_to_allocate;   //set length
	ptr++;
	*ptr = 0;   //set link
	ptr++;
	void* newPtr = ptr;
	return newPtr;
}

/* hcalloc
 * Performs the same function as hmalloc but also clears the allocated 
 * area by setting all bytes to 0.
 */
void *hcalloc(int bytes_to_allocate){
	void* ptr = hmalloc(bytes_to_allocate);
	int i = 0;
	int* ptr2 = (int*) ptr;
	for (i = 0; i < (bytes_to_allocate / 4); i++){   //zero the free area
		*ptr2 = 0;
		ptr2++;
	}
	return ptr;
}

/* hfree
 * Responsible for returning the area (pointed to by ptr) to the free
 * pool.
 *    -simply appends the returned area to the beginning of the single
 *     free list.
 */
void hfree(void *ptr){
	int* ptr2 = (int*) ptr;
	if (free_list == NULL){   //if free list is empty set the head
		ptr2--;
		ptr2--;
		free_list = ptr2;
	} else {   //append to the head of free list
		int* temp = (int*) free_list;
		ptr2--;
		ptr2--;
		free_list = ptr2;
		int dist = temp - ptr2;
		ptr2++;
		*ptr2 = dist;
	}
}

/* For the bonus credit implement hrealloc. You will need to add a prototype
 * to hmalloc.h for your function.*/

/*You may add additional functions as needed.*/
