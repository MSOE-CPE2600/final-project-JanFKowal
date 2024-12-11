/*
Contributors: Jan Kowal, Ryan Morgan
Last update: 2024/4/2024
Desc: Chat program betweeen 3 programs (Chat1.c, Chat2.c, Chat3.c). This is
a secondary program to be ran after Chat1.c to properly connect.
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

char line[BUFLEN];
struct sockaddr_in server_addr;
int sock, r;

int client_init();

int main (void)
{
  int n;
  char line[MAX], ans[MAX];
  client_init();
  while (strcmp(line,"quit")) // processing loop
  {
    printf("input a line\n");
    bzero(line, MAX);
    fgets(line, MAX, stdin);
    line[strlen(line)-1] = 0; //remove the \n
    n = write(sock, line, MAX);
    printf("client wrote %s\n", line);
    n = read(sock, ans, MAX);
    printf("client read %s\n", ans);
  }
 
  return 0;
}

int client_init()
{
  printf("create stream socket\n");
  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock < 0)
  {
    printf("socket call failed\n");
  }
  printf("fill server address with host IP and Port number\n");
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(SERVER_PORT);
  printf("connect to server");
  r = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (r<0)
  {
    printf("connect failed\n");
  }
  printf("connected ok  ...\n");
 
  printf("client init done\n");
}