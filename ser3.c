#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <time.h>

#define MAXPENDING 5    /* Maximum number of simultaneous connections */
#define BUFFSIZE 10      /* Size of message to be reeived */

#define MAX_LINES 1000
#define MAX_LINE_LENGTH 255

void err_sys(char *mess) { perror(mess); exit(1); }

int tcp_create_server_socket(int port);

int tcp_socket_listen(int socket, struct sockaddr_in echoclient);

int udp_send_message(char *ip, char *port, char *message);

int udp_receive_message(int socket, char* buffer);

int tcp_create_server_socket(int port)
{
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
  echoserver.sin_port = htons(port);     /* Server port */

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

int tcp_socket_listen(int socket, struct sockaddr_in echoclient) {
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

int udp_send_message(char *ip, char *port, char *message) {
  struct sockaddr_in echoserver;

  /* Create UDP socket */
  int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sock < 0)
  {
    err_sys("Error on socket creation");
  }

  /* Configure/set socket address for the server */
  memset(&echoserver, 0, sizeof(echoserver));    /* Erase the memory area */
  echoserver.sin_family = AF_INET;            /* Internet/IP */
  echoserver.sin_addr.s_addr = inet_addr(ip); /* IP address */
  echoserver.sin_port = htons(atoi(port));    /* Server port */

  /* Try to send the word to server */
  unsigned int echolen = strlen(message);
  int result = sendto(sock, message, echolen, 0, (struct sockaddr *)&echoserver, sizeof(echoserver));
  if (result != echolen)
  {
    err_sys("error writing word on socket");
  }
  return sock;
}

int udp_receive_message(int socket, char* buffer) {
  struct sockaddr_in echoclient;

  /* Set the maximum size of address to be received */
  int clientlen = sizeof(echoclient);

  /* Wait for echo from server */
  //fprintf(stdout, "Server echo: ");
  int received = recvfrom(socket, buffer, BUFFSIZE, 0, (struct sockaddr *)&echoclient, &clientlen);
/*  if (received != echolen)
  {
    err_sys("Error reading");
  }
*/
  /* Terminate received buffer */
  buffer[received] = '\0';

  return 0;
}



  int main(int argc, char *argv[])
  {
    struct sockaddr_in echoclient, echoserver;

    /* Check input arguments */
    if (argc != 4 && argc != 3)
    {
    fprintf(stderr, "Usge: %s <port_udp> <ip_udp> (default port_tcp=9999)\n", argv[0]);
    fprintf(stderr, "Usge: %s <port_udp> <ip_udp> <port_tcp>\n", argv[0]);
    exit(1);
    }

    int port = 9999;
    if(argc == 3)
      port = atoi(argv[3]);

    struct sockaddr_in udp_echoclient;

    int num_lines = 0;

    int udp_socket = udp_send_message(argv[2], argv[1], "lines");
    char response[BUFFSIZE];
    udp_receive_message(udp_socket, response);
    num_lines = atoi(response);
    printf("NUMBER OF LINES FROM SERVER UDP: %d\n", num_lines);

    int serversock = tcp_create_server_socket(port);

    /* As a server we are in an infinite loop, waiting forever */
    while (1)
    {

      /* Wait for a connection from a client */
      printf("Listening...\n");
      int clientsock = tcp_socket_listen(serversock, echoclient);

      char buffer[BUFFSIZE] = "";

      srand(time(NULL));
      int line_number = rand() % num_lines;
      printf("The random line number is %d\n", line_number);
      char to_send[10];
      sprintf(to_send, "%d", line_number);
      int udp_socket = udp_send_message(argv[2], argv[1], to_send);
      char response[BUFFSIZE];
      udp_receive_message(udp_socket, response);
      int number = atoi(response);

      int it = 0;

      while (strcmp(buffer, "/disc"))
      {
        read(clientsock, &buffer[0], BUFFSIZE);
        it++;
        int guess = atoi(buffer);
        if (guess > number)
        {
          char *msg = "-1";
          write(clientsock, msg, strlen(msg) + 1);
        } else if (guess < number){
        char *msg = "1";
        write(clientsock, msg, strlen(msg) + 1);
      } else {
        char *msg = "0";
        write(clientsock, msg, strlen(msg) + 1);
      }
    }

    printf("\nThe random number is %d\n", number);
    printf("Client has found the right number in %d iterations\n", it-1);

    /* Close socket */
    close(clientsock);  
  }
}