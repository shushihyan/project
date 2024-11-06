#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

void redirectInput(int argc, char** argv) {
    if (argc != 2) {
        cerr << "Usage: ./redirect-cin <input-file>" << endl;
        exit(1);
    }

    const char* filePath = argv[1];
    int fileDesc = open(filePath, O_RDONLY);
    
    if (fileDesc == -1) {
        cerr << "Error: Could not open file '" << filePath << "'" << endl;
        exit(1);
    }

    if (dup2(fileDesc, STDIN_FILENO) == -1) {
        cerr << "Error: Failed to redirect standard input" << endl;
        close(fileDesc);
        exit(1);
    }

    close(fileDesc);
}

int main(int argc, char** argv) {
    redirectInput(argc, argv);

    string input;
    cin >> input;

    string reversed(input.rbegin(), input.rend());

    cout << reversed << endl;

    return 0;
}