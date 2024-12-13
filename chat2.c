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

// Define some constants
#define MAX 1024
#define SERVER_IP "172.17.76.112" // Replace with the actual server IP
#define SERVER_PORT 1234

// Declare a buffer for messages and a structure for the server address
char line[MAX];
struct sockaddr_in server_addr;
int sock, r;

// Function to read data from the server
void *read_from_server(void *arg) {
    char ans[MAX];
    while (1) {
        // Read data from the server
        int n = read(sock, ans, MAX);
        if (n <= 0) {
            // If no data is received, close the socket and exit the thread
            printf("server disconnected\n");
            close(sock);
            pthread_exit(NULL);
        }
        // Print the received data
        printf("server: %s\n", ans);
        printf("client> \n");
        // Clear the buffer
        bzero(ans, MAX);
    }
}

// Function to write data to the server
void *write_to_server(void *arg) {
    while (1) {
        printf("client> \n");
        // Clear the buffer
        bzero(line, MAX);
        // Read input from the client's standard input
        fgets(line, MAX, stdin);
        // Remove the newline character from the input
        line[strlen(line) - 1] = 0;
        // Write the input to the server
        write(sock, line, MAX);
        // If the client sends "quit", close the socket and exit the thread
        if (!strcmp(line, "quit")) {
            close(sock);
            pthread_exit(NULL);
        }
    }
}

// Function to initialize the client
int client_init() {
    printf("create a TCP socket\n");
    // Create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("socket call failed\n");
        return -1;
    }

    printf("fill server_addr with server IP and PORT#\n");
    // Set up the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    printf("connecting to server ...\n");
    // Connect to the server
    r = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (r < 0) {
        perror("connect failed");
        return -1;
    }
    printf("connected to server\n");
    return 0;
}

int main(void) {
    // Initialize the client
    if (client_init() < 0) {
        return -1;
    }

    // Create threads for reading from and writing to the server
    pthread_t read_thread, write_thread;
    pthread_create(&read_thread, NULL, read_from_server, NULL);
    pthread_create(&write_thread, NULL, write_to_server, NULL);

    // Wait for the threads to finish
    pthread_join(read_thread, NULL);
    pthread_join(write_thread, NULL);

    // Close the client socket
    close(sock);
    return 0;
}
