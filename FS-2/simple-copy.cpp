#include <iostream>
#include <fstream>
#include <string>

const size_t BUFFER_SIZE = 16;


int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Please provide source and destination files" << std::endl;
        return 1;
    }

    std::string sourcePath = argv[1];
    std::string destinationPath = argv[2];

    std::ifstream sourceFile(sourcePath, std::ios::binary);
    if (!sourceFile) {
        std::cerr << "Error opening source file: " << sourcePath << std::endl;
        return 1;
    }

    std::ofstream destinationFile(destinationPath, std::ios::binary);
    if (!destinationFile) {
        std::cerr << "Error opening destination file: " << destinationPath << std::endl;
        return 1;
    }

    char buffer[BUFFER_SIZE];
    size_t overallBytes = 0;  

    while (sourceFile.read(buffer, BUFFER_SIZE)) {
        destinationFile.write(buffer, sourceFile.gcount());
        overallBytes += sourceFile.gcount();
    }

    if (sourceFile.gcount() > 0) {
        destinationFile.write(buffer, sourceFile.gcount());
        overallBytes += sourceFile.gcount();
    }

    sourceFile.close();
    destinationFile.close();

    std::cout << "Completed copying file. Overall bytes copied: " << overallBytes << std::endl;

    return 0;
}