/*
Contributors: Jan Kowal, Ryan Morgan
Last update: 2024/4/2024
Desc: Chat program betweeen 3 programs (Chat1.c, Chat2.c, and Chat3.c). This is
the initial program that needs to be ran first to properly connect.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

// Define some constants
#define MAX 1024
#define SERVER_PORT 1234

// Declare global variables
int mysock, csock1, r;
struct sockaddr_in server_addr, client_addr1;
socklen_t len1;
char line1[MAX];

// Function to read data from the client
void *read_from_client(void *arg) {
    while (1) {
        // Receive data from the client
        int n = recvfrom(csock1, line1, MAX, 0, NULL, NULL);
        if (n <= 0) {
            // If no data is received, close the client socket and exit the thread
            printf("server client died, server loops\n");
            close(csock1);
            pthread_exit(NULL);
        }
        // Print the received data
        printf("client: %s\n", line1);
        // If the client sends "quit", close the socket and exit the thread
        if (!strcmp(line1, "quit")) {
            close(csock1);
            pthread_exit(NULL);
        }
        printf("server> \n");
    }
}

// Function to write data to the client
void *write_to_client(void *arg) {
    while (1) {
        printf("server> \n");
        // Clear the buffer
        bzero(line1, MAX);
        // Read input from the server's standard input
        fgets(line1, MAX, stdin);
        // Remove the newline character from the input
        line1[strlen(line1) - 1] = 0;
        // Send the input to the client
        sendto(csock1, line1, strlen(line1), 0, (struct sockaddr *)&client_addr1, sizeof(client_addr1));
        // If the server sends "quit", close the socket and exit the thread
        if (!strcmp(line1, "quit")) {
            close(csock1);
            pthread_exit(NULL);
        }
    }
}

// Function to initialize the server
int server_init() {
    printf("create stream socket\n");
    // Create a socket
    mysock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (mysock < 0) {
        printf("socket call failed\n");
        return -1;
    }
    printf("fill server address with host IP and Port number\n");
    // Set up the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);
    printf("bind the socket\n");
    // Bind the socket to the server address
    r = bind(mysock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (r < 0) {
        printf("bind failed\n");
        return -1;
    }
    printf("server is listening ...\n");
    // Listen for incoming connections
    listen(mysock, 5);
    printf("server init done\n");
    return 0;
}

int main() {
    // Initialize the server
    if (server_init() < 0) {
        return -1;
    }

    while (1) {
        printf("server: accepting new connection ...\n");
        // Accept a new client connection
        len1 = sizeof(client_addr1);
        csock1 = accept(mysock, (struct sockaddr *)&client_addr1, &len1);
        if (csock1 < 0) {
            printf("server accept error\n");
        }
        printf("server accepted a client connection from\n");
        printf("Client: IP= %s port=%d\n", inet_ntoa(client_addr1.sin_addr), ntohs(client_addr1.sin_port));

        // Create threads for reading from and writing to the client
        pthread_t read_thread, write_thread;
        pthread_create(&read_thread, NULL, read_from_client, NULL);
        pthread_create(&write_thread, NULL, write_to_client, NULL);

        // Wait for the threads to finish
        pthread_join(read_thread, NULL);
        pthread_join(write_thread, NULL);
    }

    // Close the server socket
    close(mysock);
    return 0;
}
