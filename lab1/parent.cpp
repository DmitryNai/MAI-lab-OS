#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <cstring>

int main() {
    int pipe1[2], pipe2[2];

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        std::cerr << "Pipe creation failed!" << std::endl;
        return 1;
    }

    std::string file1, file2;
    std::cout << "Enter the filename for child1: ";
    std::getline(std::cin, file1);
    std::cout << "Enter the filename for child2: ";
    std::getline(std::cin, file2);

    pid_t child1 = fork();
    if (child1 == -1) {
        std::cerr << "Fork failed for child1!" << std::endl;
        return 1;
    }

    if (child1 == 0) {
        close(pipe1[1]);  
        execlp("./child", "child", std::to_string(pipe1[0]).c_str(), file1.c_str(), nullptr);
        std::cerr << "Failed to execute child1 program!" << std::endl;
        return 1;
    }

    pid_t child2 = fork();
    if (child2 == -1) {
        std::cerr << "Fork failed for child2!" << std::endl;
        return 1;
    }

    if (child2 == 0) {
        close(pipe2[1]); 
        execlp("./child", "child", std::to_string(pipe2[0]).c_str(), file2.c_str(), nullptr);
        std::cerr << "Failed to execute child2 program!" << std::endl;
        return 1;
    }

    close(pipe1[0]);  
    close(pipe2[0]);

    std::string input;
    while (true) {
        std::cout << "Enter a string (or type 'exit' to quit): ";
        std::getline(std::cin, input);

        if (input == "exit")
            break;

        if (input.length() > 10) {
            write(pipe2[1], input.c_str(), input.length() + 1);
        } else {
            write(pipe1[1], input.c_str(), input.length() + 1);
        }
    }

    close(pipe1[1]);
    close(pipe2[1]);
    std::cout << "Child processes have finished. Exiting..." << std::endl;
    return 0;
}
