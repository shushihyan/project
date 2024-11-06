#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <chrono>
#include <cstring>

void do_command(char** argv) {
       auto start = std::chrono::high_resolution_clock::now();

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Fork failed!" << std::endl;
        exit(1);
    } else if (pid == 0) {

        execvp(argv[0], argv);
        std::cerr << "Command execution failed: " << strerror(errno) << std::endl;
        exit(1);
    } else {

        int status;
        waitpid(pid, &status, 0);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        if (WIFEXITED(status)) {
            std::cout << "Command completed with exit code " << WEXITSTATUS(status)
                      << " and took " << duration.count() << " seconds." << std::endl;
        } else {
            std::cout << "Command failed." << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <command> [arguments...]" << std::endl;
        return 1;
    }

    do_command(argv + 1);
    return 0;
}