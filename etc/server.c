#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define CLNT_MAX 10
#define BUFFSIZE 200

int g_clnt_socks[CLNT_MAX];
int g_clnt_count =0;

pthread_mutex_t g_mutex;

void send_all_clnt(char * msg, int my_sock)
{
	pthread_mutex_lock(&g_mutex);
	for(int i = 0 ; i < g_clnt_count ; i++){
		if(g_clnt_socks[i] != my_sock){
			printf("send msg to all clients : %s", msg);
			write(g_clnt_socks[i], msg, strlen(msg) + 1);
		}
	}
	pthread_mutex_unlock(&g_mutex);
}

void * clnt_connection(void * arg)
{

	int clnt_sock = (int)(size_t)arg;
	int str_len = 0;

	char msg[BUFFSIZE];
	int i;

	while(1){
		str_len = read(clnt_sock, msg, sizeof(msg));
		if(str_len == -1){
			printf("clnt[%d] close\n", clnt_sock);
			break;
		}
		send_all_clnt(msg,clnt_sock);
		printf("%s\n",msg);
	}
	//	When client is closed
	pthread_mutex_lock(&g_mutex);
    for(i = 0 ; i < g_clnt_count ; i++){
		if(clnt_sock == g_clnt_socks[i]){
			for(; i < g_clnt_count - 1 ; i++)
                g_clnt_socks[i] = g_clnt_socks[i + 1];
            break;
        }//end if
    }//end for
	pthread_mutex_lock(&g_mutex);
	close(clnt_sock);

	pthread_exit(0);
	return NULL;
}

int main()
{

	int 				serv_sock;
	int 				clnt_sock;

	pthread_t 			t_thread;

	struct sockaddr_in	clnt_addr;
    unsigned int 		clnt_addr_size;

	struct sockaddr_in	serv_addr;


	pthread_mutex_init(&g_mutex, NULL);

	//	PF_INET = IPv4, SOCK_STREAM = TCP
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	
	serv_addr.sin_family = AF_INET; // IPv4
	//	converts the unsigned integer hostlong from host byte order to network byte order.
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	//	converts the unsigned short integer hostshort from host byte order to network byte order.
	serv_addr.sin_port = htons(7989);
	//	int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	//	assigns the address specified by addr to the socket referred to
    //  by the file descriptor sockfd.  addrlen specifies the size, in
    //  bytes, of the address structure pointed to by addr. 
	if(bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1){
		printf("bind error\n");
	}
	//	int listen(int sockfd, int backlog);
	//	The sockfd argument is a file descriptor that refers to a socket
    //	of type SOCK_STREAM or SOCK_SEQPACKET.
	//	The backlog argument defines the maximum length to which the
    //	queue of pending connections for sockfd may grow.
	if(listen(serv_sock, 5) == -1){
		printf("listen error");
	}

	while (1){
		clnt_addr_size = sizeof(clnt_addr);
		//	The accept() system call is used with connection-based socket
       	//	types (SOCK_STREAM, SOCK_SEQPACKET).  It extracts the first
       	//	connection request on the queue of pending connections for the
       	//	listening socket, sockfd, creates a new connected socket, and
       	//	returns a new file descriptor referring to that socket.  The
       	//	newly created socket is not in the listening state.  The original
       	//	socket sockfd is unaffected by this call.
		clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
	
		pthread_mutex_lock(&g_mutex);
		g_clnt_socks[g_clnt_count++] = clnt_sock;
		pthread_mutex_unlock(&g_mutex);

		pthread_create(&t_thread, NULL, clnt_connection, (void *)(size_t)clnt_sock);
	}
}





