#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(int argc,char **argv)
{

    int					sock;
    struct sockaddr_in	serv_addr;
    pthread_t			rcv_thread;

    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		printf("socket error");
	
	memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(7989);

    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
		printf("connect error\n");
    
	char	line[100];
 	char	msg[1000]; 
	while(1){
        fgets(line, sizeof(line), stdin);
	 	write(sock, msg, strlen(msg) + 1);

        sleep(1);
	}
    close(sock);
    return 0;
}