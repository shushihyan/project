#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 80
#define BUFFER_SIZE 4096

int main(){
    int sockfd;
    struct hostent *server;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    server = gethostbyname("httpforever.com");
    if(server == NULL) {
        fprintf(stderr, "Ошибка: не найден указанный хост\n");
        exit(EXIT_FAILURE);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        perror("Ошибка создания сокета");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    if(connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("Ошибка подключения к серверу");
        exit(EXIT_FAILURE);
    }

    char request[] = "GET / HTTP/1.1\r\nHost: httpforever.com\r\nConnection: close\r\n\r\n";
    if(send(sockfd, request, strlen(request), 0) < 0){
        perror("Ошибка отправки запроса");
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen("httpforever.html", "w");
    if(fp == NULL) {
        perror("Ошибка открытия файла для записи");
        exit(EXIT_FAILURE);
    }

    int bytes;
    while ((bytes = recv(sockfd, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytes] = '\0';
        fputs(buffer, fp);
    }
    if (bytes < 0) {
        perror("Ошибка получения ответа");
    }

    fclose(fp);
    close(sockfd);
    printf("Ответ сервера сохранен в файл httpforever.html\n");

    return 0;
}