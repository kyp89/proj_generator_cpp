#include <iostream>
#include <filesystem>

#include "projectGenerator.hpp"

int main(int argc, char* argv[]) {

    std::cout << "Hello world" << std::endl;

    std::filesystem::path appPath = std::filesystem::path(argv[0]).parent_path();

    for (int i = 0; i < argc; i++) {
        std::cout << "argv[" << i << "] = " << argv[i] << "\n";
    }
    ProjectGenerator::ProjectParams params = ProjectGenerator::getParamsFromArgs(argc, argv);
    //std::string myLibrary = "MyLibrary";
    //Only for tests
    ProjectGenerator::clearPath(params.name);
    //
    ProjectGenerator::generarteProject(params.name, params.path.string(), appPath, params);

    int val = 0;
    std::cin >> val;

    return 0;
}