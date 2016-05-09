#include <errno.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;
typedef struct
{
   int sock;
   char name[1024];
} Client;


int main (int argc, char** argv)
{
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock == -1)
    {
      perror("socket()");
      exit(errno);
    }

  SOCKADDR_IN  sin = { 0 };
  sin.sin_addr.s_addr = htonl(INADDR_ANY); /* nous sommes un serveur, nous acceptons n'importe quelle adresse */
  sin.sin_family = AF_INET;
  sin.sin_port = htons(8789);
  if(bind (sock, (SOCKADDR *) &sin, sizeof sin) == -1)
    {
      perror("bind()");
      exit(errno);
    }

  if(listen(sock, 1) == -1)
    {
      perror("listen()");
      exit(errno);
    }

  SOCKADDR_IN csin = { 0 };
  int  csock;

  socklen_t sinsize = sizeof csin;

  csock = accept(sock, ((SOCKADDR *)&csin), &sinsize);

  if(csock == -1)
    {
      perror("accept()");
      exit(errno);
    }
  printf("WE DID IT\n");
  return 0;
}
