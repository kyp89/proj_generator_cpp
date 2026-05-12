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
    inline  constexpr std::string_view PROJECT_USE_SDL = "--useSDL";
    inline  constexpr std::string_view PROJECT_USE_SDL_VAL = "yes";

    namespace CmakeParamsKeys {
        constexpr std::string_view FETCH_CONTENT = "#{{FETCH_CONTENT}}";
        constexpr std::string_view SDL_FETCH = "#---{{SDL_FETCH}}";
        constexpr std::string_view SDL_TARGET_LINK = "#{{SDL_TARGET_LINK}}";
        constexpr std::string_view SDL_COPY_DLL = "#{{SDL_COPY_DLL}}";
    }

    namespace CmakeParamsCommonValue {
        inline  constexpr std::string_view FETCH_CONTENT = "include(FetchContent)";
    }

    namespace SDLTemplateFiles {
        inline  constexpr std::string_view SDL_FETCH = "templates/SDL/sdl-fetch.template.txt";
        inline  constexpr std::string_view SDL_TARGET_LINK = "templates/SDL/sdl-link.template.txt";
        inline  constexpr std::string_view SDL_COPY_DLL = "templates/SDL/sdl-copy-dll.template.txt";
        inline  constexpr std::string_view SDL_MAIN = "templates/SDL/main.cpp";
    }

    namespace CmakeParams {
        constexpr std::string_view PROJECT_NAME = "{{PROJECT_NAME}}";
        constexpr std::string_view CMAKE_VERSION = "{{CMAKE_VERSION}}";
    }

    struct ProjectParams
    {
        std::string name = "MyNewProject";
        std::filesystem::path path = "";
        std::string cmakeVersion = "3.26";
        bool useSdl = false;
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