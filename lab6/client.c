#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define PORT 12325  
#define ADRESS "127.0.0.1"

void *receive_messages(void *arg) {
    int server_socket = *(int *)arg;
    char buffer[BUFFER_SIZE];
    int read_size;

    while ((read_size = recv(server_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[read_size] = '\0';
        printf("%s\n", buffer);
    }

    pthread_exit(NULL);
}

int main() {
    int server_socket;
    struct sockaddr_in server_addr;
    
    server_socket = socket(AF_INET, SOCK_STREAM, 0); // (Ipv4, TCP, TCP/IP)
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(1);
    }
  
    server_addr.sin_family = AF_INET; // Ipv4
    server_addr.sin_port = htons(PORT); // "host to net"
    server_addr.sin_addr.s_addr = inet_addr(ADRESS);

    if (connect(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server");
        exit(1);
    }

    char username[20];
    printf("Enter your username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';
 
    if (send(server_socket, username, sizeof(username), 0) == -1) {
        perror("Error sending username");
        close(server_socket);
        exit(1);
    }

    pthread_t thread;
    if (pthread_create(&thread, NULL, receive_messages, (void *)&server_socket) != 0) {
        perror("Error creating thread");
        close(server_socket);
        exit(1);
    }

    char buffer[BUFFER_SIZE];
    while (1) {
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (send(server_socket, buffer, strlen(buffer), 0) == -1) {
            perror("Error sending message");
            break;
        }
    }
    close(server_socket);

    return 0;
}