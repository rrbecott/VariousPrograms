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

/*Opcodes for RPC calls*/
#define open_call   1 
#define close_call  2
#define read_call   3
#define write_call  4
#define seek_call   5
#define pipe_call   6
#define dup2_call   7

/* In this project, we will develop a mini Remote Procedure Call (RPC) 
 * based system consisting of a em server and a client. Using the remote 
 * procedures supplied by the server our client program will be able to 
 * open files and perform computations on the server. 
 *
 * The server should open a socket and listen on an available port. The 
 * server program upon starting should print the port number it is using 
 * on the standard output 
 * (Print only the port number with no additional formatting. You may use printf \%d). 
 * This port number is then manually passed as a command line argument to 
 * the client to establish the connection. In order to implement the RPC, 
 * the server and the client should communicate through a TCP socket. It 
 * is allowed to fork a child for each open connection and delagate the 
 * handling to the child.
 */

/* main - server implementation
 */

int main(int argc, char *argv[]){
	int conn, length;
	char type;
	int listener = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in s1, s2;
	bzero((char*)&s1, sizeof(s1));
	s1.sin_family = AF_INET;
	s1.sin_addr.s_addr = htonl(INADDR_ANY);
	s1.sin_port = htons(0);
	bind(listener, (struct sockaddr *)&s1, sizeof(s1));
	length = sizeof(s1);
	getsockname(listener, (struct sockaddr *)&s1, (socklen_t*) &length);
	printf("%d\n", ntohs(s1.sin_port));

	listen(listener, 10);
	length = sizeof(s2);

	while(1){
		conn = accept(listener, (struct sockaddr *)&s2, (socklen_t*) &length);
		if (fork() == 0){
			int readNumI = read(conn, &type, 1);
			int L;
			while (type != EOF){
				if (readNumI < 0)
					perror("Call read error (instr)\n");
				L = 0;

				if ((int) type == open_call){
					int len, flags, mode, fd;
					char temp[4];
					if (read(conn, temp, 2) < 0)
						perror("Length read error (open)\n");
					len = (temp[0] << 8) | temp[1];
					char* pathname = malloc(len);
					if (read(conn, pathname, len) < 0)
                                                perror("Pathname read error (open)\n");
					if (read(conn, temp, 4) < 0)
                                                perror("Flags read error (open)\n");
					flags = (temp[0] << 24) | (temp[1] << 16) | (temp[2] << 8) | temp[3];
					if (read(conn, temp, 4) < 0)
                                                perror("Mode read error (open)\n");
					mode = (temp[0] << 24) | (temp[1] << 16) | (temp[2] << 8) | temp[3];

					fd = open(pathname, flags, mode);
					free(pathname);
					char rslt[8];

					rslt[L++] = (fd >> 24) & 0xff;
        				rslt[L++] = (fd >> 16) & 0xff;
        				rslt[L++] = (fd >> 8) & 0xff;
        				rslt[L++] = (fd ) & 0xff;

        				rslt[L++] = (errno >> 24) & 0xff;
        				rslt[L++] = (errno >> 16) & 0xff;
        				rslt[L++] = (errno >> 8) & 0xff;
        				rslt[L++] = (errno ) & 0xff;

					write(conn, rslt, 8);

				} else if ((int) type == close_call){

					char temp[4];
					if (read(conn, temp, 4) < 0)
                                                perror("Fd read error (close)\n");
                                        int fd = (temp[0] << 24) | (temp[1] << 16) | (temp[2] << 8) | temp[3];
					int closeNum = close(fd);
                                        char rslt[2*sizeof(int)];

					rslt[L++] = (closeNum >> 24) & 0xff;
                                        rslt[L++] = (closeNum >> 16) & 0xff;
                                        rslt[L++] = (closeNum >> 8) & 0xff;
                                        rslt[L++] = (closeNum ) & 0xff;

                                        rslt[L++] = (errno >> 24) & 0xff;
                                        rslt[L++] = (errno >> 16) & 0xff;
                                        rslt[L++] = (errno >> 8) & 0xff;
                                        rslt[L++] = (errno ) & 0xff;

                                        write(conn, rslt, 2*sizeof(int));

				} else if ((int) type == read_call){

					char temp[4];
					if (read(conn, temp, 4) < 0)
                                                perror("Fd read error (read)\n");
                                        int fd = (temp[0] << 24) | (temp[1] << 16) | (temp[2] << 8) | temp[3];
					if (read(conn, temp, 4) < 0)
                                                perror("Count read error (read)\n");
                                        int count = (temp[0] << 24) | (temp[1] << 16) | (temp[2] << 8) | temp[3];
					char* readBuf = malloc(count);
					int readNum = read(fd, readBuf, count);
					char* rslt = malloc(readNum+2*sizeof(int));

					rslt[L++] = (readNum >> 24) & 0xff;
                                        rslt[L++] = (readNum >> 16) & 0xff;
                                        rslt[L++] = (readNum >> 8) & 0xff;
                                        rslt[L++] = (readNum ) & 0xff;

                                        rslt[L++] = (errno >> 24) & 0xff;
                                        rslt[L++] = (errno >> 16) & 0xff;
                                        rslt[L++] = (errno >> 8) & 0xff;
                                        rslt[L++] = (errno ) & 0xff;

					int i;
					for (i = 0; i < readNum; i++)
						rslt[L++] = readBuf[i];
					free(readBuf);
					write(conn, rslt, readNum+2*sizeof(int));
					free(rslt);

				} else if ((int) type == write_call){

					char temp[4];
					char rslt[2*sizeof(int)];
					if (read(conn, temp, 4) < 0)
                                                perror("Fd read error (write)\n");
                                        int fd = (temp[0] << 24) | (temp[1] << 16) | (temp[2] << 8) | temp[3];
					if (read(conn, temp, 4) < 0)
                                                perror("Count read error (write)\n");
                                        int count = (temp[0] << 24) | (temp[1] << 16) | (temp[2] << 8) | temp[3];
					char* writeBuf = malloc(count);
					if (read(conn, writeBuf, count) < 0)
                                                perror("Data read error (write)\n");
					int writeNum = write(fd, writeBuf, count);
					free(writeBuf);

					rslt[L++] = (writeNum >> 24) & 0xff;
                                        rslt[L++] = (writeNum >> 16) & 0xff;
                                        rslt[L++] = (writeNum >> 8) & 0xff;
                                        rslt[L++] = (writeNum ) & 0xff;

                                        rslt[L++] = (errno >> 24) & 0xff;
                                        rslt[L++] = (errno >> 16) & 0xff;
                                        rslt[L++] = (errno >> 8) & 0xff;
                                        rslt[L++] = (errno ) & 0xff;

					write(conn, rslt, 2*sizeof(int));

				} else if ((int) type == seek_call){

					char temp[4];
					char rslt[2*sizeof(int)];
                                        if (read(conn, temp, 4) < 0)
                                                perror("Fd read error (seek)\n");
                                        int fd = (temp[0] << 24) | (temp[1] << 16) | (temp[2] << 8) | temp[3];
                                        if (read(conn, temp, 4) < 0)
                                                perror("Offset read error (seek)\n");
                                        int offset = (temp[0] << 24) | (temp[1] << 16) | (temp[2] << 8) | temp[3];
					if (read(conn, temp, 4) < 0)
                                                perror("Whence read error (seek)\n");
                                        int whence = (temp[0] << 24) | (temp[1] << 16) | (temp[2] << 8) | temp[3];
					int seekNum = lseek(fd, offset, whence);

					rslt[L++] = (seekNum >> 24) & 0xff;
                                        rslt[L++] = (seekNum >> 16) & 0xff;
                                        rslt[L++] = (seekNum >> 8) & 0xff;
                                        rslt[L++] = (seekNum ) & 0xff;

                                        rslt[L++] = (errno >> 24) & 0xff;
                                        rslt[L++] = (errno >> 16) & 0xff;
                                        rslt[L++] = (errno >> 8) & 0xff;
                                        rslt[L++] = (errno ) & 0xff;

                                        write(conn, rslt, 2*sizeof(int));

				} else if ((int) type == pipe_call){

					char rslt[4*sizeof(int)];
					int pipefd[2];
					int pipeNum = pipe(pipefd);

					rslt[L++] = (pipeNum >> 24) & 0xff;
                                        rslt[L++] = (pipeNum >> 16) & 0xff;
                                        rslt[L++] = (pipeNum >> 8) & 0xff;
                                        rslt[L++] = (pipeNum ) & 0xff;

                                        rslt[L++] = (errno >> 24) & 0xff;
                                        rslt[L++] = (errno >> 16) & 0xff;
                                        rslt[L++] = (errno >> 8) & 0xff;
                                        rslt[L++] = (errno ) & 0xff;

					rslt[L++] = (pipefd[0] >> 24) & 0xff;
                                        rslt[L++] = (pipefd[0] >> 16) & 0xff;
                                        rslt[L++] = (pipefd[0] >> 8) & 0xff;
                                        rslt[L++] = (pipefd[0] ) & 0xff;

                                        rslt[L++] = (pipefd[1] >> 24) & 0xff;
                                        rslt[L++] = (pipefd[1] >> 16) & 0xff;
                                        rslt[L++] = (pipefd[1] >> 8) & 0xff;
                                        rslt[L++] = (pipefd[1] ) & 0xff;

					write(conn, rslt, 4*sizeof(int));

				} else if ((int) type == dup2_call){

					char temp[4];
					char rslt[2*sizeof(int)];
					if (read(conn, temp, 4) < 0)
                                                perror("Oldfd read error (dup2)\n");
                                        int oldfd = (temp[0] << 24) | (temp[1] << 16) | (temp[2] << 8) | temp[3];
                                        if (read(conn, temp, 4) < 0)
                                                perror("Newfd read error (dup2)\n");
                                        int newfd = (temp[0] << 24) | (temp[1] << 16) | (temp[2] << 8) | temp[3];
					int dupNum = dup2(oldfd, newfd);

					rslt[L++] = (dupNum >> 24) & 0xff;
                                        rslt[L++] = (dupNum >> 16) & 0xff;
                                        rslt[L++] = (dupNum >> 8) & 0xff;
                                        rslt[L++] = (dupNum ) & 0xff;

                                        rslt[L++] = (errno >> 24) & 0xff;
                                        rslt[L++] = (errno >> 16) & 0xff;
                                        rslt[L++] = (errno >> 8) & 0xff;
                                        rslt[L++] = (errno ) & 0xff;

					write(conn, rslt, 2*sizeof(int));

				} else {
					return -1;
				}
				readNumI = read(conn, &type, 1);
			}
			return 1;
		}
	}

	return 1;
}
