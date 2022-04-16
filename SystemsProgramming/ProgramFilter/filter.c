//Add any includes you require.

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <signal.h>

/* main - implementation of filter
 * In this project, you will be developing a filter program that forks
 * a child and then executes a program passed as an argument and intercepts 
 * all output and input from that program. The syntax for the execution of 
 * the filter program is given by:
 * 
 * 	filter <program name> <arguments to program>
 *
 * The following commands must be implemented:
 * 	//           : Pass the character '/' as input to the program.
 * 	/i           : Go to input only mode.
 * 	/o           : Go to input/output mode.
 * 	/c           : Go to command mode.
 * 	/m <integer> : Set the max-text, maximum number of lines to be displayed.
 * 	/k <integer> : Send the child process indicated signal.
 *
 * See the spec for details.
 * 
 * After receiving each command, the program should output a prompt indicating 
 * the current mode and if there is more data to be displayed. 
 * The prompt syntax is :
 *
 * 	<pid> m <more> #
 *
 * where <pid> is the process id of the child process, m is the current mode (
 * i (input only), c (command), o(input/output)), optional <more> is the text "more" 
 * if there is data available to be displayed, and lastly the pound character.
 */
int main(int argc, char *argv[]){
	//Hint: use select() to determine when reads may execute without blocking.
	int inputPipe[2];
	int outputPipe[2];
	int errorPipe[2];
	char mode = 'c';
	int maxLine = 20;
	char* progName = argv[1];
	char buf[1000];
	int status;

	if (pipe(inputPipe) < 0 || pipe(outputPipe) < 0 || pipe(errorPipe) < 0)
		perror("Pipe creation error\n");

	pid_t pid = fork();
	if (pid == 0){
		dup2(inputPipe[0], 0);
		dup2(outputPipe[1], 1);
		dup2(errorPipe[1], 2);
		execv(progName, &argv[1]);
	} else {
		fd_set fdset;
		int sel;
		int com = 0;
		int more = 0;
		int prnt = 0;
		int died = 0;
		struct timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 1;
		while (1){
			FD_ZERO(&fdset);
			FD_SET(0, &fdset);
			FD_SET(errorPipe[0], &fdset);
			if (mode == 'o'){
				FD_SET(outputPipe[0], &fdset);
			}

			sel = select(errorPipe[1] + 1, &fdset, (fd_set *)0, (fd_set *)0, NULL);
			if (sel < 0)
				perror("Select error\n");

			if (FD_ISSET(errorPipe[0], &fdset)){
				int readNum;
				char bufC;
				while (readNum > 0){
					readNum = read(errorPipe[0], &bufC, 1);
					if (readNum < 0)
						perror("Error pipe read error\n");
					write(2, &bufC, readNum);
					waitpid(pid, &status, WNOHANG);
                                        if (died == 0){
                                                printf("The child %d has terminated with code %d\n", pid, status);
                                                died = 1;
                                        }
				}
			}

			if (FD_ISSET(0, &fdset)){
				int readNum = read(0, buf, sizeof(buf));
				int writeNum;
				if (readNum < 0)
					perror("Input read error\n");
				if (mode == 'c'){
					if (buf[0] == '/'){
						com = 1;
					} else {
						printf("In command mode, only accepts commands\n");
					}
				} else if (mode == 'i' || mode == 'o'){
					if (buf[0] == '/'){
						if (buf[1] == '/'){
							writeNum = write(inputPipe[1], buf+1, readNum-1);
							if (writeNum < 0)
								perror("Input write error\n");
						} else {
							com = 1;
						}
					} else {
						writeNum = write(inputPipe[1], buf, readNum);
						if (writeNum < 0)
                                                	perror("Input write error\n");
					}
				} else {
					perror("Mode error\n");
				}
			}

			if (com == 1){
				char newMode = buf[1];
				char cArgs[16];
				int i;
				for (i = 0; i < 16; i++){
					cArgs[i] = buf[i+3];
				}
				int inp = atoi(cArgs);
				if (newMode == 'c' || newMode == 'i' || newMode == 'o'){
					mode = newMode;
					if (mode == 'o'){
						com = 0;
					}
                                } else if(newMode == 'k'){
					kill(pid, inp);
					waitpid(pid, &status, WNOHANG);
					if (died == 0){
						printf("The child %d has terminated with code %d\n", pid, status);
                                		died = 1;
					}
                                } else if(newMode == 'm'){
					maxLine = inp;
					com = 1;
                                } else if(newMode == '/'){
					write(inputPipe[1], "/", 1);
                                } else{
					perror("Command given error\n");
				}
			}

			if (mode == 'o' && FD_ISSET(outputPipe[0], &fdset)){
				int line = 0;
				char buf[2];
				int readNum = 1;
				while(1){
					FD_ZERO(&fdset);
                                	FD_SET(outputPipe[0], &fdset);
                        		sel = select(errorPipe[1] + 1, &fdset, (fd_set *)0, (fd_set *)0, &timeout);
					if (!(FD_ISSET(outputPipe[0], &fdset)))
						break;
					readNum = read(outputPipe[0], buf, 1);
					if (readNum < 0)
						perror("Output read error\n");
					if (buf[0] == '\n')
                                                line++;
                                        write(1, buf, readNum);
                                        if (line >= maxLine){
                                                more = 1;
                                                break;
                                        }
				}
				mode = 'i';
				prnt = 1;
			} else {
				com = 1;
			}

			if (waitpid(pid, &status, WNOHANG) == -1 && died == 0){
				printf("The child %d has terminated with code %d\n", pid, status);
				died = 1;
			}

			if (com || prnt){
				printf("%d %c", pid, mode);
				if (more){
					printf(" more");
				}
				printf(" #\n");
			}

			prnt = 0;
			more = 0;
			com = 0;
		}
	}

	return 0;
}
