#include "r_client.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <strings.h>
#include <netdb.h>
#include <stdlib.h>
#include <errno.h>

/* Add any includes needed*/

/*Opcodes for RPC calls*/
#define open_call   1
#define close_call  2
#define read_call   3
#define write_call  4
#define seek_call   5
#define pipe_call   6
#define dup2_call   7

int entry(int argc, char *argv[]);
int socketfd = -1;

/* main - entry point for client applications.
 *
 * You are expected to develop a client program which will provide an 
 * environment into which we can plug an application and execute it 
 * using the remote versions of the supported calls. The client program 
 * therefore should expect a <hostname> <portnumber> pair as its first 
 * two arguments and attempt to connect the server. Once it connects, it
 * should call the user program which has a function called entry 
 * analogous to the main program in an ordinary C program. The entry 
 * routine should have the argv and argc arguments and return an integer 
 * value, just like the ordinary main. The client program should strip 
 * off the first two arguments, create a new argv array and call the entry procedure. 
 * Finally, when entry exits, the return value should be returned from the 
 * main procedure of the client.
 */
int main(int argc, char *argv[]){
	char* host = argv[1];
	unsigned short port = atoi(argv[2]);
	struct sockaddr_in remote;
	struct hostent *h;

	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero((char*) &remote, sizeof(remote));
	remote.sin_family = AF_INET;
	h = gethostbyname(host);
	bcopy((char*)h->h_addr, (char*)&remote.sin_addr, h->h_length);
	remote.sin_port = htons(port);
	connect(socketfd, (struct sockaddr*)&remote, sizeof(remote));

	char *argv2[3];
	argv2[1] = argv[3];
	argv2[2] = argv[4];
	entry(argc-2, argv2);
	char eof = EOF;
	write(socketfd, &eof, 1);

	return 1;
}

/* r_open
 * remote open
 */
int r_open(const char *pathname, int flags, int mode){
	int L;
	char * msg;
	const char * p;
	int in_msg;
	int in_err;
	int u_l;

	p=pathname;
	while(*p) p++;

	u_l = p-pathname;
	L = 1 + // this is the opcode
		2 + u_l + // 2-byte length field followed by the pathname.
		sizeof(flags) + // int bytes for flags.
		sizeof(mode); // int bytes for mode.

	msg = malloc(L);
	L=0;
	msg[L++] = open_call; // this is the code for open.
	msg[L++] = (u_l >> 8) & 0xff; // this is the length.
	msg[L++] = (u_l) & 0xff;

	int i;
	for (i=0; i < u_l; i++)
		msg[L++]=pathname[i]; // put the pathname.

	msg[L++] = (flags >> 24) & 0xff; // put the flags.
	msg[L++] = (flags >> 16) & 0xff;
	msg[L++] = (flags >> 8) & 0xff;
	msg[L++] = (flags ) & 0xff;
	msg[L++] = (mode >> 24) & 0xff; // put the mode.
	msg[L++] = (mode >> 16) & 0xff;
	msg[L++] = (mode >> 8) & 0xff;
	msg[L++] = (mode ) & 0xff;
	// This is where you can split (A).
	write(socketfd,msg,L);
	read(socketfd,msg,8);

	in_msg = (msg[0] << 24) | (msg[1] << 16) | (msg[2] << 8) | msg[3];
	in_err = (msg[4] << 24) | (msg[5] << 16) | (msg[6] << 8) | msg[7];

	free(msg);
	errno = in_err;
	return in_msg;
}

/* r_close
 * remote close
 */
int r_close(int fd){
	int L;
        char * msg;
        int in_msg;
        int in_err;

        L = 1 + // this is the opcode
                sizeof(fd);

        msg = malloc(L);
        L=0;
        msg[L++] = close_call;
        msg[L++] = (fd >> 24) & 0xff;
        msg[L++] = (fd >> 16) & 0xff;
        msg[L++] = (fd >> 8) & 0xff;
        msg[L++] = (fd ) & 0xff;
        // This is where you can split (A).
        write(socketfd,msg,L);
        read(socketfd,msg,8);

        in_msg = (msg[0] << 24) | (msg[1] << 16) | (msg[2] << 8) | msg[3];
        in_err = (msg[4] << 24) | (msg[5] << 16) | (msg[6] << 8) | msg[7];

        free(msg);
        errno = in_err;
        return in_msg;
}

/* r_read
 * remote read
 */
int r_read(int fd, void *buf, int count){
	int L;
        char * msg;
        int in_msg;
        int in_err;
	char* buf2 = (char*) buf;

        L = 1 + // this is the opcode
                sizeof(fd) +
                sizeof(count);

        msg = malloc(L);
        L=0;
        msg[L++] = read_call;
        msg[L++] = (fd >> 24) & 0xff;
        msg[L++] = (fd >> 16) & 0xff;
        msg[L++] = (fd >> 8) & 0xff;
        msg[L++] = (fd ) & 0xff;

        msg[L++] = (count >> 24) & 0xff;
        msg[L++] = (count >> 16) & 0xff;
        msg[L++] = (count >> 8) & 0xff;
        msg[L++] = (count ) & 0xff;
        // This is where you can split (A).

        write(socketfd,msg,L);
	char* temp = malloc(count+2*sizeof(int));
        read(socketfd,temp,count+2*sizeof(int));

        in_msg = (temp[0] << 24) | (temp[1] << 16) | (temp[2] << 8) | temp[3];
        in_err = (temp[4] << 24) | (temp[5] << 16) | (temp[6] << 8) | temp[7];

	int i;
	for (i = 0; i < in_msg; i++){
		buf2[i] = temp[i+2*sizeof(int)];
	}

        free(msg);
	free(temp);
        errno = in_err;
        return in_msg;
}

/* r_write
 * remote write
 */
int r_write(int fd, const void *buf, int count){
	int L;
        char * msg;
        int in_msg;
        int in_err;
	const char* temp = buf;

        L = 1 + // this is the opcode
                sizeof(fd) +
		sizeof(count) +
		count;

        msg = malloc(L);
        L=0;
        msg[L++] = write_call;
        msg[L++] = (fd >> 24) & 0xff;
        msg[L++] = (fd >> 16) & 0xff;
        msg[L++] = (fd >> 8) & 0xff;
        msg[L++] = (fd ) & 0xff;

	msg[L++] = (count >> 24) & 0xff;
        msg[L++] = (count >> 16) & 0xff;
        msg[L++] = (count >> 8) & 0xff;
        msg[L++] = (count ) & 0xff;
        // This is where you can split (A).
	int i;
	for (i = 0; i < count; i++)
		msg[L++] = temp[i];

        write(socketfd,msg,L);
        read(socketfd,msg,8);

        in_msg = (msg[0] << 24) | (msg[1] << 16) | (msg[2] << 8) | msg[3];
        in_err = (msg[4] << 24) | (msg[5] << 16) | (msg[6] << 8) | msg[7];

        free(msg);
        errno = in_err;
        return in_msg;
}

/* r_lseek
 * remote seek
 */
int r_lseek(int fd, int offset, int whence){
	int L;
        char * msg;
        int in_msg;
        int in_err;

        L = 1 +
                sizeof(fd) +
                sizeof(offset) +
                sizeof(whence);

        msg = malloc(L);
        L=0;
        msg[L++] = seek_call;
        msg[L++] = (fd >> 24) & 0xff;
        msg[L++] = (fd >> 16) & 0xff;
        msg[L++] = (fd >> 8) & 0xff;
        msg[L++] = (fd ) & 0xff;
        msg[L++] = (offset >> 24) & 0xff;
        msg[L++] = (offset >> 16) & 0xff;
        msg[L++] = (offset >> 8) & 0xff;
        msg[L++] = (offset ) & 0xff;
	msg[L++] = (whence >> 24) & 0xff;
        msg[L++] = (whence >> 16) & 0xff;
        msg[L++] = (whence >> 8) & 0xff;
        msg[L++] = (whence ) & 0xff;
        // This is where you can split (A).
        write(socketfd,msg,L);
        read(socketfd,msg,8);

        in_msg = (msg[0] << 24) | (msg[1] << 16) | (msg[2] << 8) | msg[3];
        in_err = (msg[4] << 24) | (msg[5] << 16) | (msg[6] << 8) | msg[7];

        free(msg);
        errno = in_err;
        return in_msg;
}

/* r_pipe
 * remote pipe
 */
int r_pipe(int pipefd[2]){
        char * msg;
        int in_msg;
        int in_err;
	char op = pipe_call;

        msg = malloc(4*sizeof(int));
        write(socketfd,&op,1);
        read(socketfd,msg,16);

        in_msg = (msg[0] << 24) | (msg[1] << 16) | (msg[2] << 8) | msg[3];
        in_err = (msg[4] << 24) | (msg[5] << 16) | (msg[6] << 8) | msg[7];
	pipefd[0] = (msg[8] << 24) | (msg[9] << 16) | (msg[10] << 8) | msg[11];
	pipefd[1] = (msg[8] << 24) | (msg[9] << 16) | (msg[10] << 8) | msg[11];

        free(msg);
        errno = in_err;
        return in_msg;
}

/* r_dup2
 * remote dup2
 */
int r_dup2(int oldfd, int newfd){
	int L;
        char * msg;
        int in_msg;
        int in_err;

        L = 1 +
                sizeof(oldfd) +
                sizeof(newfd);

        msg = malloc(L);
        L=0;
        msg[L++] = dup2_call;
        msg[L++] = (oldfd >> 24) & 0xff;
        msg[L++] = (oldfd >> 16) & 0xff;
        msg[L++] = (oldfd >> 8) & 0xff;
        msg[L++] = (oldfd ) & 0xff;
        msg[L++] = (newfd >> 24) & 0xff;
        msg[L++] = (newfd >> 16) & 0xff;
        msg[L++] = (newfd >> 8) & 0xff;
        msg[L++] = (newfd ) & 0xff;
        // This is where you can split (A).
        write(socketfd,msg,L);
        read(socketfd,msg,8);

	in_msg = (msg[0] << 24) | (msg[1] << 16) | (msg[2] << 8) | msg[3];
        in_err = (msg[4] << 24) | (msg[5] << 16) | (msg[6] << 8) | msg[7];

        free(msg);
        errno = in_err;
        return in_msg;
}
