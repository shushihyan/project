#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>
#include <errno.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Please provide a file path to delete." << std::endl;
        return 1;
    }

    const char* filePath = argv[1];

    struct stat fileStat;
    if (stat(filePath, &fileStat) == -1) {
        std::cerr << "Error: Cannot access file. " << strerror(errno) << std::endl;
        return 1;
    }

    int fd = open(filePath, O_WRONLY);
    if (fd == -1) {
        std::cerr << "Error: Cannot open file. " << strerror(errno) << std::endl;
        return 1;
    }

    off_t fileSize = fileStat.st_size;

    char nullByte = '\0';
    for (off_t i = 0; i < fileSize; ++i) {
        if (write(fd, &nullByte, 1) == -1) {
            std::cerr << "Error: Failed to overwrite file content. " << strerror(errno) << std::endl;
            close(fd);
            return 1;
        }
    }

    close(fd);

    if (unlink(filePath) == -1) {
        std::cerr << "Error: Failed to delete the file. " << strerror(errno) << std::endl;
        return 1;
    }

    std::cout << "File '" << filePath << "' was securely deleted." << std::endl;

    return 0;
}