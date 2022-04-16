#include <sys/socket.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
 #include <sys/select.h>
#include <string.h>

int main(int argc, char *argv[]){

int i = 0;
char buff[100];
for (i = 0; i < 30; i++){
	sprintf(buff, "line %d\n", i);
	write(1, buff, strlen(buff));
}

	char buf[100];
	read(0,buf, sizeof(buf));
	write(1, buf, strlen(buf));

//	while (1) printf("hi\n");
    printf("leaving\n");
exit(1);
    return 0;
}
