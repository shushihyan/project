#include <iostream>
#include <unistd.h>
#include <cmath>
#include <string>
#include <cstring>
#include <cstdlib>

int find_nth_prime(int n) {
    int count = 0, num = 2;
    while (count < n) {
        bool is_prime = true;
        for (int i = 2; i <= std::sqrt(num); ++i) {
            if (num % i == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) ++count;
        ++num;
    }
    return num - 1;
}

int main() {
    int parent_to_child[2], child_to_parent[2];

    if (pipe(parent_to_child) == -1 || pipe(child_to_parent) == -1) {
        perror("Pipe error");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork error");
        return 1;
    }

    if (pid == 0) {
        close(parent_to_child[1]);
        close(child_to_parent[0]);

        while (true) {
            char buffer[32];
            read(parent_to_child[0], buffer, sizeof(buffer));
            if (strcmp(buffer, "exit") == 0) {
                std::cout << "[Child] Exiting...\n";
                break;
            }

            int n = atoi(buffer);
            int result = find_nth_prime(n);

            write(child_to_parent[1], &result, sizeof(result));
        }

        close(parent_to_child[0]);
        close(child_to_parent[1]);
    } else {
        close(parent_to_child[0]);
        close(child_to_parent[1]);

        while (true) {
            std::string input;
            std::cout << "[Parent] Enter a number (or 'exit' to quit): ";
            std::getline(std::cin, input);

            write(parent_to_child[1], input.c_str(), input.size() + 1);
            if (input == "exit") break;

            int result;
            read(child_to_parent[0], &result, sizeof(result));
            std::cout << "[Parent] The " << input << "-th prime is " << result << "\n";
        }

        close(parent_to_child[1]);
        close(child_to_parent[0]);
    }

    return 0;
}