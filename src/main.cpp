#include <iostream>
#include <filesystem>

#include "projectGenerator.hpp"

int main(int argc, char* argv[]) {

    std::filesystem::path appPath = std::filesystem::absolute(argv[0]).parent_path();

    if(argc < 2 || ProjectGenerator::isHelpRequested(argc, argv)) {
        ProjectGenerator::printHelp();
        return 0;
    }

    for (int i = 0; i < argc; i++) {
        std::cout << "argv[" << i << "] = " << argv[i] << "\n";
    }

    ProjectGenerator::ProjectParams params = ProjectGenerator::getParamsFromArgs(argc, argv);
    ProjectGenerator::generateProject(appPath, params);

    return 0;
}
