/*
Contributors: Jan Kowal, Ryan Morgan
Last update: 2024/4/2024
Desc: Chat program between 3 programs (Chat1.c, Chat2.c, and Chat3.c). This is
a secondary program to be run after Chat1.c to properly connect.
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
#include <pthread.h>

// define some constants
#define MAX 1024
#define SERVER_IP "172.17.76.112" // Replace with the actual server IP
#define SERVER_PORT 1234

char line[MAX];
struct sockaddr_in server_addr;
int sock, r;

void *read_from_server(void *arg) {
    char ans[MAX];
    while (1) {
        int n = read(sock, ans, MAX);
        if (n <= 0) {
            printf("server disconnected\n");
            close(sock);
            pthread_exit(NULL);
        }
        printf("server> %s\n", ans);
        printf("client> \n");
    }
}

void *write_to_server(void *arg) {
    while (1) {
        printf("client> \n");
        bzero(line, MAX);
        fgets(line, MAX, stdin);
        line[strlen(line) - 1] = 0; // remove the \n
        int n = write(sock, line, MAX);
        if (!strcmp(line, "quit")) {
            close(sock);
            pthread_exit(NULL);
        }
    }
}

int client_init() {
    printf("create a TCP socket\n");
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("socket call failed\n");
        return -1;
    }

    printf("fill server_addr with server IP and PORT#\n");
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    printf("connecting to server ...\n");
    r = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (r < 0) {
        perror("connect failed");
        return -1;
    }
    printf("connected to server\n");
    return 0;
}

int main(void) {
    if (client_init() < 0) {
        return -1;
    }

    pthread_t read_thread, write_thread;
    pthread_create(&read_thread, NULL, read_from_server, NULL);
    pthread_create(&write_thread, NULL, write_to_server, NULL);

    pthread_join(read_thread, NULL);
    pthread_join(write_thread, NULL);

    close(sock);
    return 0;
}