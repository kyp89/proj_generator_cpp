#ifndef PROJECT_GENERATOR_H
#define PROJECT_GENERATOR_H

#include <string>
#include <iostream>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <unordered_map>

namespace ProjectGenerator {

    /*PARAMS*/
    inline  constexpr std::string_view PROJECT_NAME = "--projectName";
    inline  constexpr std::string_view PROJECT_PATH = "--projectPath";

    namespace CmakeParams {
        constexpr std::string_view PROJECT_NAME = "{{PROJECT_NAME}}";
        constexpr std::string_view CMAKE_VERSION = "{{CMAKE_VERSION}}";
    }

    struct ProjectParams
    {
        std::string name = "MyNewProject";
        std::filesystem::path path = "";
        std::string cmakeVersion = "3.26";
    };
    
    ProjectParams getParamsFromArgs(int argc, char* argv[]);
    void clearPath(const std::string projectPath);
    void generarteProject(const std::string projectName, const std::string projectPath, const std::filesystem::path appPath, const ProjectParams& projectParams);
    void copyProjectFiles(const std::filesystem::path fullProjectPath, const std::filesystem::path appPath, const ProjectParams& projectParams);
    void createProjectDirs(const std::filesystem::path fullProjectPath);
    void updateCmakeListsFile(std::filesystem::path cmakeListsFileDest, const ProjectParams& projectParams);
    void updateVSLaunchJSONFile(std::filesystem::path launchJsonPath, const ProjectParams& projectParams);
    void updateMainFile(std::filesystem::path mainFilePath, const ProjectParams& projectParams);
    void replaceAll(std::string& str, const std::string& from, const std::string& to);
    void replaceInFile(const std::string& filePath, std::unordered_map<std::string, std::string>& params);
}

#endif