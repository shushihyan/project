#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 8080

double calculate(const std::string &operation, double a, double b) {
    if (operation == "ADD") return a + b;
    if (operation == "SUB") return a - b;
    if (operation == "MUL") return a * b;
    if (operation == "DIV") return (b != 0) ? a / b : 0;
    return 0;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Ошибка создания сокета");
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Ошибка привязки");
        return 1;
    }

    
    if (listen(server_fd, 3) < 0) {
        perror("Ошибка прослушивания");
        return 1;
    }

    std::cout << "RPC-сервер запущен на порту " << PORT << std::endl;

    while (true) {
        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("Ошибка подключения");
            continue;
        }

        read(new_socket, buffer, 1024);
        std::istringstream iss(buffer);
        std::string operation;
        double a, b;

        if (iss >> operation >> a >> b) {
            double result = calculate(operation, a, b);
            std::string response = "Результат: " + std::to_string(result) + "\n";
            send(new_socket, response.c_str(), response.length(), 0);
        } else {
            std::string error = "Ошибка: неправильный ввод\n";
            send(new_socket, error.c_str(), error.length(), 0);
        }

        close(new_socket);
    }

    return 0;
}
