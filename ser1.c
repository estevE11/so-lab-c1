#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define MAXPENDING 5    /* Maximum number of simultaneous connections */
#define BUFFSIZE 5      /* Size of message to be reeived */

void err_sys(char *mess) { perror(mess); exit(1); }

int main(int argc, char *argv[]) {
  struct sockaddr_in echoserver, echoclient;
  int serversock, clientsock;
  int result;

  /* Check input arguments */
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <port>\n", argv[0]);
    exit(1);
  }

  /* Create TCP socket */
  serversock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (serversock < 0) {
    err_sys("Error socket");
  }

  /* Set information for sockaddr_in structure */
  memset(&echoserver, 0, sizeof(echoserver));       /* Reset memory */
  echoserver.sin_family = AF_INET;                  /* Internet/IP */
  echoserver.sin_addr.s_addr = htonl(INADDR_ANY);   /* Any address */
  echoserver.sin_port = htons(atoi(argv[1]));       /* Server port */

  /* Bind socket */
  result = bind(serversock, (struct sockaddr *) &echoserver, sizeof(echoserver));
  if (result < 0) {
    err_sys("Error bind");
  }

  /* Listen socket */
  result = listen(serversock, MAXPENDING);
  if (result < 0) {
    err_sys("Error listen");
  }

  /* As a server we are in an infinite loop, waiting forever */
  while (1) {
    unsigned int clientlen = sizeof(echoclient);

    /* Wait for a connection from a client */
    printf("Listening...\n");
    clientsock = accept(serversock, (struct sockaddr *) &echoclient, &clientlen);
    if (clientsock < 0) {
      err_sys("Error accept");
    }
    char* ip = inet_ntoa(echoclient.sin_addr);
    fprintf(stdout, "Client: %s\n", ip);


    char buffer[BUFFSIZE];
    int received = -1;

    /* Read from socket */
    read(clientsock, &buffer[0], BUFFSIZE);
    printf("Message from client: %s\n", buffer);

    /* Write to socket */
    write(clientsock, buffer, strlen(buffer) + 1);

    /* Close socket */
    close(clientsock);  
  }
}