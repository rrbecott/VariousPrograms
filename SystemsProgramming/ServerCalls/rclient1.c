#include "r_client.h"
#include <stdio.h>
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
 * rclient1 remotely opens an output file, locally opens an input file, 
 * copies the input file to the output file and closes both files.
 */
int entry(int argc, char* argv[]){
	//you will call the r_ functions here.
	int locFile = open(argv[1], O_RDWR, 0666);
	if (locFile < 0)
		return -1;
	int remFile = r_open(argv[2], O_RDWR | O_CREAT | O_APPEND, 0666);

	char buf;
	int readNum = read(locFile, &buf, 1);
	while (readNum > 0){
		r_write(remFile, &buf, 1);
		readNum = read(locFile, &buf, 1);
	}

	int closeNumL = close(locFile);
	if (closeNumL < 0)
                return -1;
	int closeNumR = r_close(remFile);
        if (closeNumR < 0)
                return -1;

	return 1; //placeholder
}
