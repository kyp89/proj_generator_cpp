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
    inline  constexpr std::string_view PROJECT_USE_RAYLIB = "--useRAYLIB";
    inline  constexpr std::string_view PROJECT_USE_RAYLIB_VAL = "yes";
    inline  constexpr std::string_view PROJECT_USE_JSON = "--useJSON";
    inline  constexpr std::string_view PROJECT_USE_JSON_VAL = "yes";
    inline  constexpr std::string_view PROJECT_USE_HTTP = "--useHTTP";
    inline  constexpr std::string_view PROJECT_USE_HTTP_VAL = "yes";
    inline  constexpr std::string_view PROJECT_TYPE = "--projectType";
    inline  constexpr std::string_view PROJECT_TYPE_LIB_VAL = "lib";
    inline  constexpr std::string_view PROJECT_TYPE_APP_VAL = "app";
    inline  constexpr std::string_view PROJECT_OVERWRITE = "--overwrite";
    inline  constexpr std::string_view PROJECT_OVERWRITE_VAL = "yes";
    inline  constexpr std::string_view HELP = "--help";
    inline  constexpr std::string_view HELP_SHORT = "-h";

    namespace CmakeParamsKeys {
        constexpr std::string_view FETCH_CONTENT = "#{{FETCH_CONTENT}}";
        constexpr std::string_view SDL_FETCH = "#{{SDL_FETCH}}";
        constexpr std::string_view SDL_TARGET_LINK = "#{{SDL_TARGET_LINK}}";
        constexpr std::string_view SDL_COPY_DLL = "#{{SDL_COPY_DLL}}";
        constexpr std::string_view RAYLIB_FETCH = "#{{RAYLIB_FETCH}}";
        constexpr std::string_view RAYLIB_TARGET_LINK = "#{{RAYLIB_TARGET_LINK}}";
        constexpr std::string_view JSON_FETCH = "#{{JSON_FETCH}}";
        constexpr std::string_view JSON_TARGET_LINK = "#{{JSON_TARGET_LINK}}";
        constexpr std::string_view HTTP_FETCH = "#{{HTTP_FETCH}}";
        constexpr std::string_view HTTP_TARGET_LINK = "#{{HTTP_TARGET_LINK}}";
        constexpr std::string_view LIBRARY_TARGET = "#{{LIBRARY_TARGET}}";
        constexpr std::string_view LIBRARY_LINK = "#{{LIBRARY_LINK}}";
    }

    namespace CmakeParamsCommonValue {
        inline  constexpr std::string_view FETCH_CONTENT = "include(FetchContent)";
    }

    namespace SDLTemplateFiles {
        inline  constexpr std::string_view SDL_FETCH = "SDL/sdl-fetch.template.txt";
        inline  constexpr std::string_view SDL_TARGET_LINK = "SDL/sdl-link.template.txt";
        inline  constexpr std::string_view SDL_COPY_DLL = "SDL/sdl-copy-dll.template.txt";
        inline  constexpr std::string_view SDL_MAIN = "SDL/main.cpp";
    }

    namespace RAYLIBTemplateFiles {
        inline  constexpr std::string_view RAYLIB_FETCH = "RAYLIB/raylib-fetch.template.txt";
        inline  constexpr std::string_view RAYLIB_TARGET_LINK = "RAYLIB/raylib-link.template.txt";
        inline  constexpr std::string_view RAYLIB_MAIN = "RAYLIB/main.cpp";
    }

    namespace JSONTemplateFiles {
        inline  constexpr std::string_view JSON_FETCH = "JSON/json-fetch.template.txt";
        inline  constexpr std::string_view JSON_TARGET_LINK = "JSON/json-link.template.txt";
    }

    namespace HTTPTemplateFiles {
        inline  constexpr std::string_view HTTP_FETCH = "HTTP/http-fetch.template.txt";
        inline  constexpr std::string_view HTTP_TARGET_LINK = "HTTP/http-link.template.txt";
    }

    namespace LIBTemplateFiles {
        inline  constexpr std::string_view LIBRARY_TARGET = "LIB/lib-target.template.txt";
        inline  constexpr std::string_view LIBRARY_LINK = "LIB/lib-link.template.txt";
        inline  constexpr std::string_view LIBRARY_HEADER = "LIB/library.hpp";
        inline  constexpr std::string_view LIBRARY_SOURCE = "LIB/library.cpp";
        inline  constexpr std::string_view LIBRARY_MAIN = "LIB/main.cpp";
    }

    namespace CmakeParams {
        constexpr std::string_view PROJECT_NAME = "{{PROJECT_NAME}}";
        constexpr std::string_view CMAKE_VERSION = "{{CMAKE_VERSION}}";
        constexpr std::string_view LINK_TARGET = "{{LINK_TARGET}}";
        constexpr std::string_view LINK_SCOPE = "{{LINK_SCOPE}}";
    }

    enum class ProjectType {
        App,
        Library
    };

    struct ProjectParams
    {
        std::string name = "MyNewProject";
        std::filesystem::path path = "";
        std::string cmakeVersion = "3.26";
        bool useSdl = false;
        bool useRayLib = false;
        bool useJson = false;
        bool useHttp = false;
        ProjectType type = ProjectType::App;
        bool overwrite = false;
    };
    
    ProjectParams getParamsFromArgs(int argc, char* argv[]);
    bool isHelpRequested(int argc, char* argv[]);
    void printHelp();
    void clearPath(const std::filesystem::path projectPath);
    void generateProject(const std::filesystem::path appPath, const ProjectParams& projectParams);
    void copyProjectFiles(const std::filesystem::path fullProjectPath, const std::filesystem::path appPath, const ProjectParams& projectParams);
    void createProjectDirs(const std::filesystem::path fullProjectPath);
    void updateCmakeListsFile(std::filesystem::path cmakeListsFileDest, const std::filesystem::path templatesDir, const ProjectParams& projectParams);
    void updateVSLaunchJSONFile(std::filesystem::path launchJsonPath, const ProjectParams& projectParams);
    void updateMainFile(std::filesystem::path mainFilePath, const ProjectParams& projectParams);
    void copyLibraryFiles(const std::filesystem::path fullProjectPath, const std::filesystem::path templatesDir, const ProjectParams& projectParams);
    std::string getTemplateContent(const std::string& filePath, std::unordered_map<std::string, std::string>& params);
    void replaceAll(std::string& str, const std::string& from, const std::string& to);
    void replaceInFile(const std::string& filePath, std::unordered_map<std::string, std::string>& params);
    std::string getFileContent(const std::string& filePath);
}

#endif