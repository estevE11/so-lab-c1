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

int create_server_socket(int port);

int socket_listen(int socket, struct sockaddr_in echoclient);

int create_server_socket(int port) {
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

int* read_file_and_create_array(char* filename, int* lines) {
    FILE *fp;
    char buffer[MAX_LINE_LENGTH];
    int* line_lengths;
    int num_lines = 0;

    line_lengths = malloc(MAX_LINES * sizeof(int));

    fp = fopen(filename, "r");

    while (fgets(buffer, MAX_LINE_LENGTH, fp))
    {
      int line_length = strlen(buffer);
      if (line_length > 0 && buffer[line_length - 1] == '\n')
      {
        line_length--;
      }
      line_lengths[num_lines++] = line_length;
    }

    fclose(fp);
    *lines = num_lines;
    return line_lengths;
}

int main(int argc, char *argv[])
{
  struct sockaddr_in echoclient;

  /* Check input arguments */
  if (argc != 2 && argc != 1)
  {
      fprintf(stderr, "Usage: %s (default port=8888)\n", argv[0]);
      fprintf(stderr, "Usage: %s <port>\n", argv[0]);
      exit(1);
  }

  int port = 8888;
  if (argc == 2)
  {
      port = atoi(argv[1]);
  }

  int num_lines = 0;
  int *line_lengths = read_file_and_create_array("x.txt", &num_lines);
  int serversock = create_server_socket(port);

  /* As a server we are in an infinite loop, waiting forever */
  while (1)
  {

    /* Wait for a connection from a client */
    printf("Listening...\n");
    int clientsock = socket_listen(serversock, echoclient);

    char buffer[BUFFSIZE] = "";

    srand(time(NULL));
    int number = line_lengths[rand() % num_lines];
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

    printf("The random number is %d\n", number);
    printf("Client has found the right number in %d iterations\n", it-1);

    /* Close socket */
    close(clientsock);  
  }
}