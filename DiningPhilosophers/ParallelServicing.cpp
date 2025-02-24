#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>
#include <mutex>

std::mutex mtx;

void handle_client(int client_socket, struct sockaddr_in client_address) {
    char buffer[1001];
    while (true) {
        int rs = recv(client_socket, buffer, 1000, 0);
        if (rs == -1) {
            perror("client socket connection error");
            break;
        } else if (rs == 0) {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "Client " << inet_ntoa(client_address.sin_addr) << " disconnected\n";
            break;
        }

        if (rs > 0) {
            buffer[rs] = '\0';
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "Got message from " << inet_ntoa(client_address.sin_addr) << ":\n";
            std::cout << buffer << "\n";
        }
    }

    close(client_socket);
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket creation error");
        exit(errno);
    }
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(8888);
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("bind failed");
        exit(errno);
    }
    if (listen(server_socket, 10) < 0) {
        perror("listen failed");
        exit(errno);
    }
    std::cout << "Waiting for connection\n";
    std::vector<std::thread> threads;

    while (true) {
        int client_socket;
        struct sockaddr_in client_address;
        unsigned int client_addr_len = sizeof(client_address);
        if ((client_socket = accept(server_socket, (struct sockaddr*) &client_address, &client_addr_len)) < 0) {
            perror("accept failed");
            continue;
        }
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Connected client with address: " << inet_ntoa(client_address.sin_addr) << "\n";
        threads.emplace_back(handle_client, client_socket, client_address);
    }
    for (auto &thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    close(server_socket);
    return 0;
}