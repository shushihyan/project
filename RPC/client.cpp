#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Ошибка создания сокета");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Неверный адрес/Адрес не поддерживается");
        return 1;
    }

    
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Ошибка подключения");
        return 1;
    }

    std::string input;
    std::cout << "Введите команду (например, ADD 5 3): ";
    std::getline(std::cin, input);

    send(sock, input.c_str(), input.length(), 0);
    read(sock, buffer, 1024);

    std::cout << "Ответ сервера: " << buffer;

    close(sock);
    return 0;
}
