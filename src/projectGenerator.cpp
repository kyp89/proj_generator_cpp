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

    void generarteProject(const std::string projectName, const std::string projectPath, const std::filesystem::path appPath) {
        std::cout << "generarteProject :" << projectPath << ", Project Name: " << projectName << std::endl;

        try {
            const std::string fullProjectPath = projectPath == "" ? projectName : projectPath + "//" + projectName;
            if(std::filesystem::exists(fullProjectPath)) {
                std::cout << "Folders already Exsist: " << fullProjectPath << std::endl;
                return; 
            }
            std::filesystem::create_directories(fullProjectPath);
            std::string srcPath = fullProjectPath + "//src";
            std::filesystem::create_directories(srcPath);
            std::string includePath = fullProjectPath + "//include";
            std::filesystem::create_directories(includePath);
            std::string vscodePath = fullProjectPath + "//.vscode";
            std::filesystem::create_directories(vscodePath);
            std::string testsPath = fullProjectPath + "//tests";
            std::filesystem::create_directories(testsPath);

            std::filesystem::path _fullProjectPath = fullProjectPath;
            std::filesystem::path templatesDir = appPath / "templates";

            std::filesystem::path cmakeListsFileSrc = templatesDir / "CMakeLists.txt";
            std::filesystem::path cmakeListsFileDest = _fullProjectPath / "CMakeLists.txt";
            std::filesystem::copy_file(
                cmakeListsFileSrc,
                cmakeListsFileDest,
                std::filesystem::copy_options::overwrite_existing
            );

            std::filesystem::path gitIgnoreFileSrc = templatesDir / ".gitignore";
            std::filesystem::path gitIgnoreFileDest = _fullProjectPath / ".gitignore";
            std::filesystem::copy_file(
                gitIgnoreFileSrc,
                gitIgnoreFileDest,
                std::filesystem::copy_options::overwrite_existing
            );


        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
}