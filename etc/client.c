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

void * rev(void * arg)
{
	int		sock = (int)(size_t)arg;
	char	buff[500];
	int		len;

	while(1){
		len = read(sock, buff, sizeof(buff));
		if(len == -1){
			printf("sock close\n");
			break;
		}
		printf("%s", buff);
	}
	pthread_exit(0);
	return 0;
}

int main(int argc,char **argv)
{

    int					sock;
    struct sockaddr_in	serv_addr;
    pthread_t			rcv_thread;

	if(argc < 2){
		printf("you have to enter ID\n");
		return 0;
	}

	char	id[100];
	strcpy(id, argv[1]);
	printf("id : %s\n",id);

    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1){
		printf("socket error");
	}
	
	memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(7989);

    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1){
		printf("connect error\n");
	}

	pthread_create(&rcv_thread, NULL , rev, (void *)(size_t)sock);

	char	line[100];
 	char	msg[1000]; 
	while(1){
		//char *line = readline("\e[0;32send :\e[0m ");
		fgets(line, sizeof(line), stdin);
		printf("send : %s", line);

		sprintf(msg, "[%s] : %s", id, line);
	 	write(sock, msg, strlen(msg) + 1);

		sleep(1);
	}
    close(sock);
    return 0;
}
