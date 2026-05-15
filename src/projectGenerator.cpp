#include "projectGenerator.hpp"

namespace ProjectGenerator {
    void clearPath(const std::string projectPath) {
        try {
            if(std::filesystem::exists(projectPath)) {
                std::filesystem::remove_all(projectPath);
            }
            std::cout << "Folder/file was removed" << projectPath <<std::endl;
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }

    void generarteProject(const std::string projectName, const std::string projectPath, const std::filesystem::path appPath, const ProjectParams& projectParams) {
        std::cout << "generarteProject :" << projectPath << ", Project Name: " << projectName << std::endl;

        try {
            const std::filesystem::path fullProjectPath = projectPath == "" ? projectName : projectPath + "//" + projectName;
            if(std::filesystem::exists(fullProjectPath)) {
                std::cout << "Folders already Exsist: " << fullProjectPath.string() << std::endl;
                return; 
            }
            createProjectDirs(fullProjectPath);
            copyProjectFiles(fullProjectPath, appPath, projectParams);

            std::filesystem::path _fullProjectPath = fullProjectPath;



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
            updateCmakeListsFile(cmakeListsFileDest, projectParams);

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
                std::filesystem::path mainFileSrc = templatesDir / "SDL" / "main.cpp";
                std::filesystem::path mainFileDest = fullProjectPath / "tests" /"main.cpp";
                std::filesystem::copy_file(
                    mainFileSrc,
                    mainFileDest,
                    std::filesystem::copy_options::overwrite_existing
                );
            } else if(projectParams.useRayLib) {
                std::filesystem::path mainFileSrc = templatesDir / "RAYLIB" / "main.cpp";
                std::filesystem::path mainFileDest = fullProjectPath / "tests" /"main.cpp";
                std::filesystem::copy_file(
                    mainFileSrc,
                    mainFileDest,
                    std::filesystem::copy_options::overwrite_existing
                );
            } else {
                std::filesystem::path mainFileSrc = templatesDir / "main.cpp";
                std::filesystem::path mainFileDest = fullProjectPath / "tests" /"main.cpp";
                std::filesystem::copy_file(
                    mainFileSrc,
                    mainFileDest,
                    std::filesystem::copy_options::overwrite_existing
                );
                updateMainFile(mainFileDest, projectParams);
            }
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }

    ProjectParams getParamsFromArgs(int argc, char* argv[]) {
        ProjectParams projectParams;
        for(int i = 0; i < argc; i++) {
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
        }
        return projectParams;
    }

    void updateCmakeListsFile(std::filesystem::path cmakeListsFileDest, const ProjectParams& projectParams) {
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
        if(projectParams.useSdl || projectParams.useRayLib) {
            params[std::string(CmakeParamsKeys::FETCH_CONTENT)] = std::string(CmakeParamsCommonValue::FETCH_CONTENT);
        }
        if(projectParams.useSdl) {
            params[std::string(CmakeParamsKeys::SDL_FETCH)] = getFileContent(std::string(SDLTemplateFiles::SDL_FETCH));
            params[std::string(CmakeParamsKeys::SDL_TARGET_LINK)] = getFileContent(std::string(SDLTemplateFiles::SDL_TARGET_LINK));
            params[std::string(CmakeParamsKeys::SDL_COPY_DLL)] = getFileContent(std::string(SDLTemplateFiles::SDL_COPY_DLL));
        } else if(projectParams.useSdl == false) {
            params[std::string(CmakeParamsKeys::SDL_FETCH)] = "";
            params[std::string(CmakeParamsKeys::SDL_TARGET_LINK)] = "";
            params[std::string(CmakeParamsKeys::SDL_COPY_DLL)] = "";
        }
        if(projectParams.useRayLib) {
            params[std::string(CmakeParamsKeys::RAYLIB_FETCH)] = getFileContent(std::string(RAYLIBTemplateFiles::RAYLIB_FETCH));
            params[std::string(CmakeParamsKeys::RAYLIB_TARGET_LINK)] = getFileContent(std::string(RAYLIBTemplateFiles::RAYLIB_TARGET_LINK));
        } else if(projectParams.useRayLib == false) {
            params[std::string(CmakeParamsKeys::RAYLIB_FETCH)] = "";
            params[std::string(CmakeParamsKeys::RAYLIB_TARGET_LINK)] = "";
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
    
}