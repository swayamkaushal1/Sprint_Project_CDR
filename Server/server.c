#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <signal.h>
#define PORT 8080
#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024
int client_count = 0;
int server_running = 1;
int server_sock;
void *handle_client(void *socket_desc);
void *monitor_input(void *arg);
void cleanup_and_exit() {
    printf("\nShutting down server...\n");
    close(server_sock);
    exit(0);
}
// Thread to handle server terminal input
void *monitor_input(void *arg) {
    char input[BUFFER_SIZE];
    while (server_running) {
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';  // Remove newline
        if (strcasecmp(input, "EXIT") == 0) {
            server_running = 0;
            cleanup_and_exit();
        }
    }
    return NULL;
}
void *handle_client(void *socket_desc) {
    int client_sock = *(int *)socket_desc;
    int client_id = __sync_fetch_and_add(&client_count, 1);  // Thread-safe increment
    printf("Client connected with ID: %d\n", client_id);
    char buffer[BUFFER_SIZE];
    int choice;
    int running = 1;
    int client_value = 0;  // Each client has its own value
    while (running) {
        // Send menu
        char menu[] = "\n====== MENU ======\n"
                      "1. Change integer value\n"
                      "2. Display current value\n"
                      "3. Exit\n"
                      "Enter your choice: ";
        send(client_sock, menu, strlen(menu), 0);
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = recv(client_sock, buffer, sizeof(buffer), 0);
        if (bytes_read <= 0) {
            printf("Client %d disconnected.\n", client_id);
            break;
        }
        choice = atoi(buffer);
        memset(buffer, 0, sizeof(buffer));
        switch (choice) {
            case 1: {
                char prompt[] = "Enter new integer value: ";
                send(client_sock, prompt, strlen(prompt), 0);
                memset(buffer, 0, BUFFER_SIZE);
                recv(client_sock, buffer, sizeof(buffer), 0);
                client_value = atoi(buffer);
                snprintf(buffer, sizeof(buffer), "Value updated to %d\n", client_value);
                break;
            }
            case 2: {
                snprintf(buffer, sizeof(buffer), "Current value: %d\n", client_value);
                break;
            }
            case 3:
                snprintf(buffer, sizeof(buffer), "Goodbye!\n");
                running = 0;
                break;
            default:
                snprintf(buffer, sizeof(buffer), "Invalid choice.\n");
                break;
        }
        send(client_sock, buffer, strlen(buffer), 0);
    }
    close(client_sock);
    printf("Connection with client %d closed.\n", client_id);
    free(socket_desc);
    pthread_exit(NULL);
}
int main() {
    struct sockaddr_in server, client;
    socklen_t client_size = sizeof(struct sockaddr_in);
    int client_sock, *new_sock;
    // Create socket
    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    // Bind
    if (bind(server_sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    // Listen
    if (listen(server_sock, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Server started on port %d\n", PORT);
    printf("Type 'EXIT' anytime to stop the server safely.\n");
    // Create a thread to monitor input
    pthread_t monitor_thread;
    pthread_create(&monitor_thread, NULL, monitor_input, NULL);
    pthread_detach(monitor_thread);
    // Accept client connections
    while (server_running && (client_sock = accept(server_sock, (struct sockaddr *)&client, &client_size))) {
        pthread_t thread_id;
        new_sock = malloc(sizeof(int));
        *new_sock = client_sock;
        if (pthread_create(&thread_id, NULL, handle_client, (void *)new_sock) < 0) {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }
        pthread_detach(thread_id);
    }
    if (client_sock < 0 && server_running) {
        perror("Accept failed");
    }
    cleanup_and_exit();
    return 0;
}
 