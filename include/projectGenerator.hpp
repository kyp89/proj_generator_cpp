#ifndef PROJECT_GENERATOR_H
#define PROJECT_GENERATOR_H

#include <string>
#include <iostream>
#include <filesystem>

namespace ProjectGenerator {
    void clearPath(const std::string projectPath);
    void generarteProject(const std::string projectName, const std::string projectPath, const std::filesystem::path appPath);
}

#endif