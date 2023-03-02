#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define BUFFSIZE 255

void err_sys(char *mess) { perror(mess); exit(1); }

int create_socket(char* ip, int port);

int send_message(int socket, char* msg);

int get_center(int a, int b);

int create_socket(char* ip, int port) {
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
  echoserver.sin_port = htons(port);       /* Server port */

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

int getcenter(int a, int b) {
  return ((b - a) / 2 + a);
}

int main(int argc, char *argv[]) {

  /* Check input arguments */
  if (argc > 3) {
    fprintf(stderr, "Usage: %s <ip_server> <port>\n", argv[0]);
    fprintf(stderr, "Usage: %s (default ip=127.0.0.1) <port>\n", argv[0]);
    fprintf(stderr, "Usage: %s (default ip=127.0.0.1) (default port=8888)\n", argv[0]);
    exit(1);
  }

  char *ip = "127.0.0.1";
  int port = 8888;
  if(argc > 1) {
    port = atoi(argv[1]);
  } else if(argc > 2) {
    ip = argv[1];
    port = atoi(argv[2]);
  }

  /* Try to create TCP socket */

  int socket = create_socket(ip, port);

  int max = 100;
  int x = max / 2;
  int lo = 0, hi = max;

  int found = 0;

  char input[5];
  int response = -1;
  int it = 0;

  while(response != 0) {
    it++;
    // Enviamos numero
    sprintf(input, "%d", x); // passar de string a int i guardar en "input"
    send_message(socket, input);

    // Respuesta
    char buffer[BUFFSIZE];
    read(socket, buffer, BUFFSIZE);

    response = atoi(buffer);
    //fprintf(stdout, "%d", response);

    // calculate next number
    if (response < 0) {
      printf("Selected number in the range [%d-%d]: %d\n", lo,hi,x);
      printf("your choice is bigger than number: moving down interval\n\n");
      hi = x;
      x = getcenter(lo, x);
    } else if (response > 0) {
      printf("Selected number in the range [%d-%d]: %d\n", lo, hi, x);
      printf("your choice is lower than number: moving up interval\n\n");
      lo = x;
      x = getcenter(x, hi);
    } else {
      printf("%d, The number is CORRECT!\n", x);
      printf("You have found the right number in %d iterations!\n", it);
      break;
    }
  }

  send_message(socket, "/disc");

  /* Close socket */
  close(socket);


  exit(0);
}