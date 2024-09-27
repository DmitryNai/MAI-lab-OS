#include <iostream>
#include <fstream>
#include <algorithm>
#include <unistd.h>
#include <string>
#include <fcntl.h>

void removeVowels(std::string &str) {
    std::string vowels = "AEIOUaeiou";
    str.erase(std::remove_if(str.begin(), str.end(),
                             [&](char c) { return vowels.find(c) != std::string::npos; }),
              str.end());
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: child <pipe_fd> <filename>" << std::endl;
        return 1;
    }

    int pipe_fd = std::stoi(argv[1]);  
    std::string filename = argv[2];  

    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return 1;
    }

    char buffer[256];
    while (true) {
        ssize_t bytes_read = read(pipe_fd, buffer, sizeof(buffer));
        if (bytes_read <= 0) break; 

        std::string str(buffer);
        removeVowels(str); 

        outfile << str << std::endl;  
    }

    outfile.close();
    close(pipe_fd);
    return 0;
}