#include "r_client.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <strings.h>
#include <netdb.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

/* Add any includes needed*/

/* entry
 * This is essentially the "main" for any user program linked with
 * r_client. Main in r_client will establish the connection with the
 * server then call entry. From entry, we can implement the desired 
 * user program which may call any of the r_ RPC functions.
 *
 * rclient2 should open a local file as output and a remote file as input. 
 * It should seek the remote file to position 10 and copy the rest 
 * to the local file.
 */
int entry(int argc, char* argv[]){
	//you will call the r_ functions here.
	int locFile = open(argv[2], O_RDWR | O_CREAT, 0666);
	int remFile = r_open(argv[1], O_RDWR, 0666);
	if (remFile < 0)
		return -1;

	r_lseek(remFile, 10, SEEK_CUR);

	char buf;
	int readNum = r_read(remFile, &buf, 1);
	while(readNum > 0){
		write(locFile, &buf, 1);
		readNum = r_read(remFile, &buf, 1);
	}

	return 1; //placeholder
}
