#include "hmalloc.h"
#include <stdio.h>
/*You may include any other relevant headers here.*/


/*	main()
 *	Use this function to develop tests for hmalloc. You should not 
 *	implement any of the hmalloc functionality here. That should be
 *	done in hmalloc.c
 *	This file will not be graded. When grading I will replace main 
 *	with my own implementation for testing.*/
/*int main(int argc, char *argv[]){
	// some calls to hmalloc
	// ...
	// some calls to hfree
	void* ptr1 = hmalloc(32);
	void* ptr2 = hmalloc(64);
	void* ptr3 = hmalloc(16);
	void* ptr4 = hmalloc(128);
	void* ptr5 = hmalloc(8);
        hfree(ptr2);
	hfree(ptr1);
	hfree(ptr5);
	void* ptr6 = hmalloc(1024);
	void* ptr7 = hmalloc(4);
	hfree(ptr6);
	void* ptr8 = hmalloc(1024);
	hfree(ptr8);
	void* ptr9 = hmalloc(1024);
	hfree(ptr9);
	void* ptr10 = hmalloc(1024);
	hfree(ptr10);
	traverse();
	return 1;
}*/

struct S1 {
  int a;
  int b;
} s1, s3;

struct S2 {
  int b;
  int a;
} *s2;

typedef struct S1 S;

S s4;

main()
{
  s1.a = 10;
  s1.b = 20;
  s2 = &s1;
  s3 = s1;
  s4 = s1;
  printf("s2.a = %d; s2.b = %d\n", s2->a, s2->b);
  printf("s3.a = %d; s3.b = %d\n", s3.a, s3.b);
  printf("s4.a = %d; s4.b = %d\n", s4.a, s4.b);
}
