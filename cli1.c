#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define BUFFSIZE 255

void err_sys(char *mess) { perror(mess); exit(1); }

int create_socket(char* ip, char* port);

int send_message(int socket, char* msg);

int create_socket(char* ip, char* port) {
  struct sockaddr_in echoserver;
  int sock, result;

  sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock < 0) {
    err_sys("Error socket");
  }

  /* Set information for sockaddr_in */
  memset(&echoserver, 0, sizeof(echoserver));       /* Reset memory */
  echoserver.sin_family = AF_INET;                  /* Internet/IP */
  echoserver.sin_addr.s_addr = inet_addr(ip);  /* Server address */
  echoserver.sin_port = htons(atoi(port));       /* Server port */

  /* Try to have a connection with the server */
  result = connect(sock, (struct sockaddr *) &echoserver, sizeof(echoserver));
  if (result < 0) {
    err_sys("Error connect");
  }

  return sock;
}

int send_message(int socket, char* msg) {
  write(socket, msg, strlen(msg) + 1);
  return 0;
}

int main(int argc, char *argv[]) {

  /* Check input arguments */
  if (argc != 4) {
    fprintf(stderr, "Usage: %s <ip_server> <word> <port>\n", argv[0]);
    exit(1);
  }

  /* Try to create TCP socket */

  int socket = create_socket(argv[1], argv[3]);

  char input[5]; 
  do {
    if(strcmp(input, "") != 0) {
      send_message(socket, input);

      /* Read from socket */
      //read(sock, buffer, BUFFSIZE);
      //fprintf(stdout, " %s ...done \n", buffer);
    }

    printf("Enter a message: ");
    scanf("%s", input);
  } while(strcmp(input, "exit"));

  send_message(socket, "/disc");

  /* Close socket */
  close(socket);


  exit(0);
}