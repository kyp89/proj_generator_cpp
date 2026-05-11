#include <iostream>
#include <filesystem>

#include "projectGenerator.hpp"

int main(int argc, char* argv[]) {

    std::cout << "Hello world" << std::endl;

    std::filesystem::path appPath = std::filesystem::path(argv[0]).parent_path();

    for (int i = 0; i < argc; i++) {
        std::cout << "argv[" << i << "] = " << argv[i] << "\n";
    }
    std::string myLibrary = "MyLibrary";
    //Only for tests
    ProjectGenerator::clearPath(myLibrary);
    //
    ProjectGenerator::generarteProject(myLibrary, "", appPath);

    int val = 0;
    std::cin >> val;

    return 0;
}