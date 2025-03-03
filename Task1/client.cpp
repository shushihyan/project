#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 12345
#define BUFFER_SIZE 1024
#define NAME_LEN 32

int sockfd;
char name[NAME_LEN];

void *recv_handler(void *arg) {
    char buffer[BUFFER_SIZE];
    while (1) {
        int len = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (len <= 0) break;
        buffer[len] = '\0';
        printf("%s", buffer);
    }
    return NULL;
}

int main() {
    struct sockaddr_in serv_addr;
    printf("Enter name: ");
    fgets(name, NAME_LEN, stdin);
    name[strcspn(name, "\n")] = 0;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) exit(1);
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) exit(1);
    
    write(sockfd, name, strlen(name));
    
    pthread_t tid;
    pthread_create(&tid, NULL, recv_handler, NULL);
    
    char buffer[BUFFER_SIZE];
    while (1) {
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        write(sockfd, buffer, strlen(buffer));
        if (strcmp(buffer, "/exit") == 0)
            break;
    }
    
    close(sockfd);
    return 0;
}