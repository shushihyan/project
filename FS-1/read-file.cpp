#include <iostream>
#include <cstdio>

int main(int argc,char* argv[]){

    if(argc<2){
        std::cout<<"please enter a file name.\n";
        return 1;
    }

    FILE* file = fopen(argv[1], "r");

    if(file == nullptr){
        std::cout << "Failed to open the file.\n";
    }

    char buffer[259];

    while(fgets(buffer, sizeof(buffer), file)){
        std::cout << buffer;
    }
    
    fclose(file);
    
    return 0;
}