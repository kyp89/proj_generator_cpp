#include "projectGenerator.hpp"

namespace ProjectGenerator {
    void clearPath(const std::filesystem::path projectPath) {
        try {
            if(std::filesystem::exists(projectPath)) {
                std::filesystem::remove_all(projectPath);
                std::cout << "Folder/file was removed: " << projectPath.string() << std::endl;
            }
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }

    void generateProject(const std::filesystem::path appPath, const ProjectParams& projectParams) {
        std::cout << "generateProject: " << projectParams.path.string() << ", Project Name: " << projectParams.name << std::endl;

        try {
            const std::filesystem::path fullProjectPath = projectParams.path / projectParams.name;
            if(std::filesystem::exists(fullProjectPath)) {
                if(!projectParams.overwrite) {
                    std::cout << "Folders already exist: " << fullProjectPath.string()
                              << " (use " << PROJECT_OVERWRITE << " " << PROJECT_OVERWRITE_VAL << " to replace)" << std::endl;
                    return;
                }
                clearPath(fullProjectPath);
            }
            createProjectDirs(fullProjectPath);
            copyProjectFiles(fullProjectPath, appPath, projectParams);
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }

    void createProjectDirs(const std::filesystem::path fullProjectPath) {
        std::filesystem::create_directories(fullProjectPath);
        std::filesystem::path srcPath = fullProjectPath / "src";
        std::filesystem::create_directories(srcPath);
        std::filesystem::path includePath = fullProjectPath  / "include";
        std::filesystem::create_directories(includePath);
        std::filesystem::path vscodePath = fullProjectPath / ".vscode";
        std::filesystem::create_directories(vscodePath);
        std::filesystem::path testsPath = fullProjectPath / "tests";
        std::filesystem::create_directories(testsPath);
    }

    void copyProjectFiles(const std::filesystem::path fullProjectPath, const std::filesystem::path appPath, const ProjectParams& projectParams) {
        try {
            std::filesystem::path templatesDir = appPath / "templates";

            std::filesystem::path cmakeListsFileSrc = templatesDir / "CMakeLists.txt";
            std::filesystem::path cmakeListsFileDest = fullProjectPath / "CMakeLists.txt";
            std::filesystem::copy_file(
                cmakeListsFileSrc,
                cmakeListsFileDest,
                std::filesystem::copy_options::overwrite_existing
            );
            updateCmakeListsFile(cmakeListsFileDest, templatesDir, projectParams);

            std::filesystem::path gitIgnoreFileSrc = templatesDir / ".gitignore";
            std::filesystem::path gitIgnoreFileDest = fullProjectPath / ".gitignore";
            std::filesystem::copy_file(
                gitIgnoreFileSrc,
                gitIgnoreFileDest,
                std::filesystem::copy_options::overwrite_existing
            );

            std::filesystem::path launchJsonFileSrc = templatesDir / "launch.json";
            std::filesystem::path launchJsonFileDest = fullProjectPath / ".vscode" /"launch.json";
            std::filesystem::copy_file(
                launchJsonFileSrc,
                launchJsonFileDest,
                std::filesystem::copy_options::overwrite_existing
            );

            updateVSLaunchJSONFile(launchJsonFileDest, projectParams);

            std::filesystem::path tasksJsonFileSrc = templatesDir / "tasks.json";
            std::filesystem::path tasksJsonFileDest = fullProjectPath / ".vscode" /"tasks.json";
            std::filesystem::copy_file(
                tasksJsonFileSrc,
                tasksJsonFileDest,
                std::filesystem::copy_options::overwrite_existing
            );

            if(projectParams.useSdl) {
                std::filesystem::path mainFileSrc = templatesDir / SDLTemplateFiles::SDL_MAIN;
                std::filesystem::path mainFileDest = fullProjectPath / "tests" /"main.cpp";
                std::filesystem::copy_file(
                    mainFileSrc,
                    mainFileDest,
                    std::filesystem::copy_options::overwrite_existing
                );
            } else if(projectParams.useRayLib) {
                std::filesystem::path mainFileSrc = templatesDir / RAYLIBTemplateFiles::RAYLIB_MAIN;
                std::filesystem::path mainFileDest = fullProjectPath / "tests" /"main.cpp";
                std::filesystem::copy_file(
                    mainFileSrc,
                    mainFileDest,
                    std::filesystem::copy_options::overwrite_existing
                );
            } else {
                std::filesystem::path mainFileSrc = projectParams.type == ProjectType::Library
                    ? templatesDir / LIBTemplateFiles::LIBRARY_MAIN
                    : templatesDir / "main.cpp";
                std::filesystem::path mainFileDest = fullProjectPath / "tests" /"main.cpp";
                std::filesystem::copy_file(
                    mainFileSrc,
                    mainFileDest,
                    std::filesystem::copy_options::overwrite_existing
                );
                updateMainFile(mainFileDest, projectParams);
            }

            if(projectParams.type == ProjectType::Library) {
                copyLibraryFiles(fullProjectPath, templatesDir, projectParams);
            }
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }

    void copyLibraryFiles(const std::filesystem::path fullProjectPath, const std::filesystem::path templatesDir, const ProjectParams& projectParams) {
        std::filesystem::path headerFileSrc = templatesDir / LIBTemplateFiles::LIBRARY_HEADER;
        std::filesystem::path headerFileDest = fullProjectPath / "include" / (projectParams.name + ".hpp");
        std::filesystem::copy_file(
            headerFileSrc,
            headerFileDest,
            std::filesystem::copy_options::overwrite_existing
        );
        updateMainFile(headerFileDest, projectParams);

        std::filesystem::path sourceFileSrc = templatesDir / LIBTemplateFiles::LIBRARY_SOURCE;
        std::filesystem::path sourceFileDest = fullProjectPath / "src" / (projectParams.name + ".cpp");
        std::filesystem::copy_file(
            sourceFileSrc,
            sourceFileDest,
            std::filesystem::copy_options::overwrite_existing
        );
        updateMainFile(sourceFileDest, projectParams);
    }

    ProjectParams getParamsFromArgs(int argc, char* argv[]) {
        ProjectParams projectParams;
        for(int i = 0; i + 1 < argc; i++) {
            if(argv[i] == ProjectGenerator::PROJECT_NAME) {
                projectParams.name = argv[i+1];
            }
            if(argv[i] == ProjectGenerator::PROJECT_PATH) {
                projectParams.path = argv[i+1];
            }
            if(argv[i] == ProjectGenerator::PROJECT_USE_SDL) {
                projectParams.useSdl = argv[i+1] == ProjectGenerator::PROJECT_USE_SDL_VAL;
            }
            if(argv[i] == ProjectGenerator::PROJECT_USE_RAYLIB) {
                projectParams.useRayLib = argv[i+1] == ProjectGenerator::PROJECT_USE_RAYLIB_VAL;
            }
            if(argv[i] == ProjectGenerator::PROJECT_USE_JSON) {
                projectParams.useJson = argv[i+1] == ProjectGenerator::PROJECT_USE_JSON_VAL;
            }
            if(argv[i] == ProjectGenerator::PROJECT_USE_HTTP) {
                projectParams.useHttp = argv[i+1] == ProjectGenerator::PROJECT_USE_HTTP_VAL;
            }
            if(argv[i] == ProjectGenerator::PROJECT_TYPE) {
                projectParams.type = argv[i+1] == ProjectGenerator::PROJECT_TYPE_LIB_VAL
                    ? ProjectType::Library
                    : ProjectType::App;
            }
            if(argv[i] == ProjectGenerator::PROJECT_OVERWRITE) {
                projectParams.overwrite = argv[i+1] == ProjectGenerator::PROJECT_OVERWRITE_VAL;
            }
        }
        return projectParams;
    }

    bool isHelpRequested(int argc, char* argv[]) {
        for(int i = 1; i < argc; i++) {
            if(argv[i] == ProjectGenerator::HELP || argv[i] == ProjectGenerator::HELP_SHORT) {
                return true;
            }
        }
        return false;
    }

    void printHelp() {
        std::cout << R"(Usage: ProjGenCpp [options]

Options:
  --projectName <name>      Project name (default: MyNewProject)
  --projectPath <path>      Directory where the project is created (default: current directory)
  --projectType <app|lib>   app - executable, lib - library + test executable (default: app)
  --useSDL yes              Add SDL3
  --useRAYLIB yes           Add raylib
  --useJSON yes             Add nlohmann/json (JSON)
  --useHTTP yes             Add cpr (HTTP/HTTPS requests)
  --overwrite yes           Remove and regenerate the project if it already exists
  --help, -h                Show this help

Example:
  ProjGenCpp --projectName MyLib --projectType lib --useJSON yes --useHTTP yes
)";
    }

    void updateCmakeListsFile(std::filesystem::path cmakeListsFileDest, const std::filesystem::path templatesDir, const ProjectParams& projectParams) {
        std::unordered_map<std::string, std::string> params = {
            {
                std::string(CmakeParams::PROJECT_NAME),
                std::string(projectParams.name)
            },
            {
                std::string(CmakeParams::CMAKE_VERSION),
                projectParams.cmakeVersion
            }
        };
        //Add fetch_Content
        if(projectParams.useSdl || projectParams.useRayLib || projectParams.useJson || projectParams.useHttp) {
            params[std::string(CmakeParamsKeys::FETCH_CONTENT)] = std::string(CmakeParamsCommonValue::FETCH_CONTENT);
        } else {
            params[std::string(CmakeParamsKeys::FETCH_CONTENT)] = "";
        }
        //Biblioteka: zależności linkowane PUBLIC do biblioteki, exe dostaje je przechodnio
        const bool isLibrary = projectParams.type == ProjectType::Library;
        std::unordered_map<std::string, std::string> linkParams = {
            {
                std::string(CmakeParams::LINK_TARGET),
                isLibrary ? "${PROJECT_NAME}Lib" : "${PROJECT_NAME}"
            },
            {
                std::string(CmakeParams::LINK_SCOPE),
                isLibrary ? "PUBLIC" : "PRIVATE"
            }
        };
        if(isLibrary) {
            params[std::string(CmakeParamsKeys::LIBRARY_TARGET)] = getFileContent((templatesDir / LIBTemplateFiles::LIBRARY_TARGET).string());
            params[std::string(CmakeParamsKeys::LIBRARY_LINK)] = getFileContent((templatesDir / LIBTemplateFiles::LIBRARY_LINK).string());
        } else {
            params[std::string(CmakeParamsKeys::LIBRARY_TARGET)] = "";
            params[std::string(CmakeParamsKeys::LIBRARY_LINK)] = "";
        }
        if(projectParams.useSdl) {
            params[std::string(CmakeParamsKeys::SDL_FETCH)] = getFileContent((templatesDir / SDLTemplateFiles::SDL_FETCH).string());
            params[std::string(CmakeParamsKeys::SDL_TARGET_LINK)] = getTemplateContent((templatesDir / SDLTemplateFiles::SDL_TARGET_LINK).string(), linkParams);
            params[std::string(CmakeParamsKeys::SDL_COPY_DLL)] = getFileContent((templatesDir / SDLTemplateFiles::SDL_COPY_DLL).string());
        } else if(projectParams.useSdl == false) {
            params[std::string(CmakeParamsKeys::SDL_FETCH)] = "";
            params[std::string(CmakeParamsKeys::SDL_TARGET_LINK)] = "";
            params[std::string(CmakeParamsKeys::SDL_COPY_DLL)] = "";
        }
        if(projectParams.useRayLib) {
            params[std::string(CmakeParamsKeys::RAYLIB_FETCH)] = getFileContent((templatesDir / RAYLIBTemplateFiles::RAYLIB_FETCH).string());
            params[std::string(CmakeParamsKeys::RAYLIB_TARGET_LINK)] = getTemplateContent((templatesDir / RAYLIBTemplateFiles::RAYLIB_TARGET_LINK).string(), linkParams);
        } else if(projectParams.useRayLib == false) {
            params[std::string(CmakeParamsKeys::RAYLIB_FETCH)] = "";
            params[std::string(CmakeParamsKeys::RAYLIB_TARGET_LINK)] = "";
        }
        if(projectParams.useJson) {
            params[std::string(CmakeParamsKeys::JSON_FETCH)] = getFileContent((templatesDir / JSONTemplateFiles::JSON_FETCH).string());
            params[std::string(CmakeParamsKeys::JSON_TARGET_LINK)] = getTemplateContent((templatesDir / JSONTemplateFiles::JSON_TARGET_LINK).string(), linkParams);
        } else {
            params[std::string(CmakeParamsKeys::JSON_FETCH)] = "";
            params[std::string(CmakeParamsKeys::JSON_TARGET_LINK)] = "";
        }
        if(projectParams.useHttp) {
            params[std::string(CmakeParamsKeys::HTTP_FETCH)] = getFileContent((templatesDir / HTTPTemplateFiles::HTTP_FETCH).string());
            params[std::string(CmakeParamsKeys::HTTP_TARGET_LINK)] = getTemplateContent((templatesDir / HTTPTemplateFiles::HTTP_TARGET_LINK).string(), linkParams);
        } else {
            params[std::string(CmakeParamsKeys::HTTP_FETCH)] = "";
            params[std::string(CmakeParamsKeys::HTTP_TARGET_LINK)] = "";
        }
        replaceInFile(cmakeListsFileDest.string(), params);
    }

    void updateVSLaunchJSONFile(std::filesystem::path launchJsonPath, const ProjectParams& projectParams) {
        std::unordered_map<std::string, std::string> params = {
            {
                std::string(CmakeParams::PROJECT_NAME),
                std::string(projectParams.name)
            }
        };
        replaceInFile(launchJsonPath.string(), params);
    }

    void updateMainFile(std::filesystem::path mainFilePath, const ProjectParams& projectParams) {
        std::unordered_map<std::string, std::string> params = {
            {
                std::string(CmakeParams::PROJECT_NAME),
                std::string(projectParams.name)
            }
        };
        replaceInFile(mainFilePath.string(), params);
    }

    void replaceAll(std::string& str, const std::string& from, const std::string& to) {
            if (from.empty()) {
                return;
            }

            std::size_t startPos = 0;

            while ((startPos = str.find(from, startPos)) != std::string::npos) {
                str.replace(startPos, from.length(), to);
                startPos += to.length();
            }
    }

    void replaceInFile(const std::string& filePath, std::unordered_map<std::string, std::string>& params)
        {
            // Odczyt pliku
            // std::ifstream input(filePath);
            // std::stringstream buffer;
            // buffer << input.rdbuf();
            // std::string content = buffer.str();
            // input.close();
            std::string content = getFileContent(filePath);

            // Podmiana
            for(auto [key, value]: params) {
                replaceAll(content, key, value);
            }
            
            // Zapis pliku
            std::ofstream output(filePath);
            output << content;
    }

    std::string getFileContent(const std::string& filePath) {
        std::ifstream input(filePath);
        std::stringstream buffer;
        buffer << input.rdbuf();
        std::string content = buffer.str();
        input.close();
        return content;
    }

    std::string getTemplateContent(const std::string& filePath, std::unordered_map<std::string, std::string>& params) {
        std::string content = getFileContent(filePath);
        for(const auto& [key, value]: params) {
            replaceAll(content, key, value);
        }
        return content;
    }
    
}