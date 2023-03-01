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

int create_server_socket(char* port);

int socket_listen(int socket, struct sockaddr_in echoclient);

int create_server_socket(char* port) {
  struct sockaddr_in echoserver;
  int serversock, clientsock;
  int result;

  /* Create TCP socket */
  serversock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (serversock < 0)
  {
    err_sys("Error socket");
  }

  /* Set information for sockaddr_in structure */
  memset(&echoserver, 0, sizeof(echoserver));     /* Reset memory */
  echoserver.sin_family = AF_INET;                /* Internet/IP */
  echoserver.sin_addr.s_addr = htonl(INADDR_ANY); /* Any address */
  echoserver.sin_port = htons(atoi(port));     /* Server port */

  /* Bind socket */
  result = bind(serversock, (struct sockaddr *)&echoserver, sizeof(echoserver));
  if (result < 0)
  {
    err_sys("Error bind");
  }

  /* Listen socket */
  result = listen(serversock, MAXPENDING);
  if (result < 0)
  {
    err_sys("Error listen");
  }

  return serversock;
}

int socket_listen(int socket, struct sockaddr_in echoclient) {
  unsigned int clientlen = sizeof(echoclient);

  int clientsock = accept(socket, (struct sockaddr *)&echoclient, &clientlen);
  if (clientsock < 0)
  {
    err_sys("Error accept");
  }
  char *ip = inet_ntoa(echoclient.sin_addr);
  fprintf(stdout, "Client: %s\n", ip);
  return clientsock;
}

int main(int argc, char *argv[]) {
  struct sockaddr_in echoclient;

  /* Check input arguments */
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <port>\n", argv[0]);
    exit(1);
  }

  int serversock = create_server_socket(argv[1]);

  /* As a server we are in an infinite loop, waiting forever */
  while (1) {

    /* Wait for a connection from a client */
    printf("Listening...\n");
    int clientsock = socket_listen(serversock, echoclient);

    char buffer[BUFFSIZE];

    int number = 0;

    while(strcmp(buffer, "/disc")) {
      /* Read from socket */
      read(clientsock, &buffer[0], BUFFSIZE);
      int guess = atoi(buffer);
      if(guess == number) {
        printf("Correct!\n");
      } else
        printf("Wrong!\n");
      // printf("Client: %s\n", buffer);
    }

    /* Write to socket */
    //write(clientsock, buffer, strlen(buffer) + 1);

    /* Close socket */
    close(clientsock);  
  }
}