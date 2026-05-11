#ifndef PROJECT_GENERATOR_H
#define PROJECT_GENERATOR_H

#include <string>
#include <iostream>
#include <filesystem>

namespace ProjectGenerator {

    /*PARAMS*/
    inline  constexpr std::string_view PROJECT_NAME = "--projectName";
    inline  constexpr std::string_view PROJECT_PATH = "--projectPath";

    struct ProjectParams
    {
        std::string name = "MyNewProject";
        std::filesystem::path path = "";
    };
    
    ProjectParams getParamsFromArgs(int argc, char* argv[]);
    void clearPath(const std::string projectPath);
    void generarteProject(const std::string projectName, const std::string projectPath, const std::filesystem::path appPath);
}

#endif