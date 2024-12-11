/*
Contributors: Jan Kowal, Ryan Morgan
Last update: 2024/4/2024
Desc: Chat program betweeen 3 programs (Chat1.c, Chat2.c, and Chat3.c). This is
the initial program that needs to be ran first to properly connect.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// define some constants
#define MAX 256
#define SERVER_HOST "localhost"
#define SERVER_IP "172.17.76.112" //"127.0.0.1" $hostname -I
#define BUFLEN 256
#define SERVER_PORT 1234

char line1[BUFLEN];
char line2[BUFLEN];
struct sockaddr_in server_addr, client_addr1, client_addr2;
int mysock, csock1, csock2, r, len1, len2, n1, n2;

int server_init();

int main (void)
{
  char line[MAX];
  server_init();
  while (1) {
    printf("server: accepting new connection ...\n");
    len1 = sizeof(client_addr1);
    len2 = sizeof(client_addr2);
    csock1 = accept(mysock, (struct sockaddr *)&client_addr1, &len1);
    if (csock1 < 0)
    {
      printf("server accept error \n");
    }
    printf("server accepted a client connection from \n");
    printf("Client: IP= %s port=%d \n", inet_ntoa(client_addr1.sin_addr), ntohs(client_addr1.sin_port));
    csock2 = accept(mysock, (struct sockaddr *)&client_addr2, &len2);
    if (csock2 < 0)
    {
      printf("server accept error \n");
    }
    printf("server accepted a client connection from \n");
    printf("Client: IP= %s port=%d \n", inet_ntoa(client_addr2.sin_addr), ntohs(client_addr2.sin_port));
 
  while(1) {
    n1 = recvfrom(csock1, line1, MAX, 0, NULL, NULL);
    if (n1 == 0)
    {
      printf("server client died, server loops\n");
      close(csock1);
      break;
    }
    printf("server read %s\n", line1);
    n1 = sendto(csock1, line1, MAX, 0, (struct sockaddr *)&client_addr1, sizeof(client_addr1));
    printf("server wrote %s\n", line1);
    if (!strcmp(line1,"quit")) {
      return 0;
    }
    n2 = recvfrom(csock2, line2, MAX, 0, NULL, NULL);
    if (n2 == 0)
    {
      printf("server client died, server loops\n");
      close(csock2);
      break;
    }
    printf("server read %s\n", line2);
    n2 = sendto(csock2, line2, MAX, 0, (struct sockaddr *)&client_addr2, sizeof(client_addr2));
    printf("server wrote %s\n", line2);
    if (!strcmp(line2,"quit")) { 
      return 0;
    }
  }
  }
  return 0;
}

int server_init()
{
  printf("create stream socket\n");
  mysock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (mysock < 0)
  {
    printf("socket call failed\n");
  }
  printf("fill server address with host IP and Port number\n");
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(SERVER_PORT);
  printf("bind the socket");
  r = bind(mysock, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (r<0)
  {
    printf("bind failed\n");
  }
  printf("server is listening  ...\n");
  listen(mysock, 5); // queue length of 5
  printf("server init done\n");
}