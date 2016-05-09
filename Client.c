#include <errno.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

int main (int argc, char** argv)
{
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock == -1)
    {
      perror("socket()");
      exit(errno);
    }

  SOCKADDR_IN sin = { 0 };
  sin.sin_addr = *(IN_ADDR *) "127.0.0.1";
  sin.sin_port = htons(8789); /* on utilise htons pour le port */
  sin.sin_family = AF_INET;
  if(connect(sock,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == -1)
    {
      perror("connect()");
      exit(errno);
    }

    return 0;
}
