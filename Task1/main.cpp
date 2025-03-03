#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 12345
#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024
#define NAME_LEN 32

typedef struct {
    int sockfd;
    int uid;
    char name[NAME_LEN];
} client_t;

client_t *clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
static int uid = 1;

void add_client(client_t *cl) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!clients[i]) {
            clients[i] = cl;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void remove_client(int uid) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] && clients[i]->uid == uid) {
            clients[i] = NULL;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void send_message_all(char *message, int sender_uid) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] && clients[i]->uid != sender_uid) {
            write(clients[i]->sockfd, message, strlen(message));
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void *handle_client(void *arg) {
    char buffer[BUFFER_SIZE];
    client_t *cli = (client_t *)arg;

    if (recv(cli->sockfd, cli->name, NAME_LEN, 0) <= 0) {
        close(cli->sockfd);
        free(cli);
        pthread_exit(NULL);
    }
    
    char msg[BUFFER_SIZE];
    sprintf(msg, "%s joined the chat\n", cli->name);
    send_message_all(msg, cli->uid);

    while (1) {
        int len = recv(cli->sockfd, buffer, BUFFER_SIZE, 0);
        if (len > 0) {
            buffer[len] = '\0';
            if (strcmp(buffer, "/exit") == 0) {
                sprintf(buffer, "%s left the chat\n", cli->name);
                send_message_all(buffer, cli->uid);
                break;
            } else if (strcmp(buffer, "/list") == 0) {
                char list[BUFFER_SIZE] = "Connected: ";
                pthread_mutex_lock(&clients_mutex);
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (clients[i]) {
                        strcat(list, clients[i]->name);
                        strcat(list, " ");
                    }
                }
                pthread_mutex_unlock(&clients_mutex);
                write(cli->sockfd, list, strlen(list));
            } else {
                char message[BUFFER_SIZE];
                sprintf(message, "%s: %s\n", cli->name, buffer);
                send_message_all(message, cli->uid);
            }
        } else {
            break;
        }
    }

    close(cli->sockfd);
    remove_client(cli->uid);
    free(cli);
    pthread_exit(NULL);
}

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) exit(1);
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);
    
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) exit(1);
    listen(sockfd, 10);
    
    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0) continue;
        
        client_t *cli = (client_t *)malloc(sizeof(client_t));
        cli->sockfd = newsockfd;
        cli->uid = uid++;
        add_client(cli);
        
        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, (void *)cli);
    }
    return 0;
}