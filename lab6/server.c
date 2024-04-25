#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_CLIENTS 3
#define BUFFER_SIZE 1024
#define PORT 12325
#define ADRESS "127.0.0.1"

typedef struct {
    int client_socket;
    char username[20];
} Client;

Client clients[MAX_CLIENTS];
pthread_t threads[MAX_CLIENTS];
size_t num_clients = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *arg) {
    Client *client = (Client *)arg;
    char buffer[BUFFER_SIZE];
    char full_message[BUFFER_SIZE];
    int read_size;
    
    while ((read_size = recv(client->client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[read_size] = '\0';
        snprintf(full_message, sizeof(full_message), "[%s]: %s", client->username, buffer);
        pthread_mutex_lock(&mutex);
            for (size_t i = 0; i < num_clients; i++) {
                if (clients[i].client_socket != client->client_socket) {
                    send(clients[i].client_socket, full_message, strlen(full_message), 0);
                }
            }
        pthread_mutex_unlock(&mutex);
        printf("%s\n", full_message);
    }

    pthread_mutex_lock(&mutex);
    for (size_t i = 0; i < num_clients; i++) {
        if (clients[i].client_socket == client->client_socket) {
            clients[i] = clients[num_clients - 1];
            break;
        }
    }
    num_clients--;
    pthread_mutex_unlock(&mutex);
    printf("- Client %s is disconnected\n", client->username);
    close(client->client_socket);
    free(client);
    pthread_exit(NULL);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
   
    server_socket = socket(AF_INET, SOCK_STREAM, 0); // (Ipv4, TCP, TCP/IP)
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET; // Ipv4
    server_addr.sin_port = htons(PORT); // "host to net"
    server_addr.sin_addr.s_addr = inet_addr(ADRESS);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding");
        exit(1);
    }

    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("Socket listening error");
        exit(1);
    }

    printf("The server is running. Waiting for connections...\n");

    while (1) {
        addr_size = sizeof(client_addr);

        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);
        if (client_socket == -1) {
            perror("Error accepting connection");
            continue;
        }
        
        char username[20];
        if (recv(client_socket, username, sizeof(username), 0) <= 0) {
            perror("Error retrieving username");
            close(client_socket);
            continue;
        }

        Client *client = (Client *)malloc(sizeof(Client));
        client->client_socket = client_socket;
        strncpy(client->username, username, sizeof(client->username));

        pthread_mutex_lock(&mutex);
        if (num_clients < MAX_CLIENTS) {
            int is_name_unique = 1;
            for (size_t i = 0; i < num_clients; i++) {
                if (strcmp(clients[i].username, client->username) == 0) {
                    char reject_msg[100] = "Connection refused. Name is not unique\n";
                    send(client->client_socket, reject_msg, strlen(reject_msg), 0);
                    close(client_socket);
                    is_name_unique = 0;
                    break;
                }
            }
            if (is_name_unique) {
                clients[num_clients] = *client;
                num_clients++;
                printf("+ Client %s is connected\n", client->username);
            }
        } else {
            printf("The maximum number of clients has been exceeded. Client %s rejected\n", client->username);
            char reject_msg[100] = "Connection refused. Too many clients\n";
            send(client->client_socket, reject_msg, strlen(reject_msg), 0);
            free(client);
            close(client_socket);
            continue;
        }
        pthread_mutex_unlock(&mutex);

        if (pthread_create(&threads[num_clients - 1], NULL, handle_client, (void *)client) != 0) {
            perror("Error creating thread");
            free(client);
            close(client_socket);
            continue;
        }
    }

    close(server_socket);

    return 0;
}