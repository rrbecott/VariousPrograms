#include "recordio.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Develop a test program called testio which includes recordio.h and 
 * is linked against recordio.o. This program should expect a single 
 * argument which supplies a file name. The program should rio_open 
 * the supplied argument and report any errors to the user. Upon a 
 * successful open it should execute a series of rio_read statements, 
 * read the file one record at a time and write each record to the 
 * standard output as shown below.
 *
 * Data File                          Index file
 * ---------------------------------------------------
 *  Systems                            0, 8
 *  programming is cool.               8, 20
 *
 * "make testio" will compile this souce into testio
 * To execute your program: ./testio <some_record_file_name>
 */
int main(int argc, char *argv[]){
	char * record;
	int d, n;

	d = rio_open("myfile.txt", O_RDWR | O_CREAT, 0600);
	n = rio_write(d, "Systems\n",8);
	n = rio_write(d, "programming is cool\n",20);
	n = rio_lseek(d,1,SEEK_SET);
	record = rio_read(d, &n);
	write(1,record,n);
	free(record);
	return 0;
}
