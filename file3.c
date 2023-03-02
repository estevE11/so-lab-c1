#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <time.h>

#define BUFFSIZE 255

#define MAX_LINES 1000
#define MAX_LINE_LENGTH 255

void err_sys(char *mess) {
  perror(mess);
  exit(1);
}

int *read_file_and_create_array(char *filename, int *lines);

int *read_file_and_create_array(char *filename, int *lines) {
  FILE *fp;
  char buffer[MAX_LINE_LENGTH];
  int *line_lengths;
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

int main(int argc, char *argv[]) {
  struct sockaddr_in echoserver, echoclient;
  unsigned int echolen, clientlen, serverlen;
  char buffer[BUFFSIZE];
  int sock, result;
  int received = 0;

  /* Check input arguments */
  if (argc != 3 && argc != 2) {
    fprintf(stderr, "Usage: %s <file> <port>\n", argv[0]);
    exit(1);
  }

  int port = 9999;
  if(argc == 3)
    port = atoi(argv[2]);

  int num_lines = 0;
  int *line_lengths = read_file_and_create_array(argv[1], &num_lines);

  /* Create UDP socket */
  sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sock < 0) {
    err_sys("Error socket");
  }

  /* Prepare sockaddr_in structure for server address */
  memset(&echoserver, 0, sizeof(echoserver));      /* Erase the memory area */
  echoserver.sin_family = AF_INET;                 /* Internet/IP */
  //echoserver.sin_addr.s_addr = inet_addr(argv[1]); /* Receive message only to that IP on server */
  // echoserver.sin_addr.s_addr = htonl(INADDR_ANY); /* Receive requests to any IP address valid on server */
  echoserver.sin_port = htons(port); /* Server port */

  /* Get size of echoserver structure */
  serverlen = sizeof(echoserver);

  /* Bind that socket with the OS, to be able to receive messages on that socket */
  result = bind(sock, (struct sockaddr *)&echoserver, serverlen);
  if (result < 0) {
    err_sys("Error bind");
  }

  printf("process file %s\n", argv[1]);
  printf("number of lines processed: %d\n", num_lines);
  
  /* As a server we are in an infinite loop, waiting forever */
  while (1) {
    /* Set the maximum size for address */
    clientlen = sizeof(echoclient);

    /* Receive a message from a particular client */
    received = recvfrom(sock, buffer, BUFFSIZE, 0, (struct sockaddr *)&echoclient, &clientlen);
    if (received < 0) {
      err_sys("Error receiveing word from client");
    }

    /* Terminate received buffer */
    buffer[received] = '\0';

    // ip: inet_ntoa(echoclient.sin_addr)
    /* Print client address */
    fprintf(stdout, "REQUEST FROM SERVER UDP: %s\n", inet_ntoa(echoclient.sin_addr));
    if(strcmp(buffer, "lines") == 0) {
      printf("Number of lines in file: %d\n", num_lines);
      sprintf(buffer, "%d", num_lines);
    } else {
      int line_number = atoi(buffer);
      printf("Random number: %d\n", line_number);
      sprintf(buffer, "%d", line_lengths[line_number]);
    }
    /* Try to send echo word back to the client */
    result = sendto(sock, buffer, received, 0, (struct sockaddr *)&echoclient, sizeof(echoclient));
    if (result != received) {
      err_sys("Error writing message back to the client");
    }
  }
}