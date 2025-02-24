#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
    struct sockaddr_in server_address, client_address;
    socklen_t client_len = sizeof(client_address);
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket creation error");
        exit(errno);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8888);
    if (bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
        perror("bind failed");
        exit(errno);
    }
    if (listen(server_socket, 5) == -1) {
        perror("listen failed");
        exit(errno);
    }

    std::cout << "Server is listening on port 8888..." << std::endl;
    int client_socket = accept(server_socket, (struct sockaddr*) &client_address, &client_len);
    if (client_socket == -1) {
        perror("accept failed");
        exit(errno);
    }

    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (received == -1) {
            perror("recv failed");
            break;
        } else if (received == 0) {
            std::cout << "Client disconnected" << std::endl;
            break;
        }

        std::cout << "Received: " << buffer << std::endl;
    }

    close(client_socket);
    close(server_socket);
    return 0;
}