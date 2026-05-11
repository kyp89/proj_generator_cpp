cmake_minimum_required(VERSION 3.26)
project(PROJ_GEN_CPP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Wymuszamy tryb Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_library(ProjGenCppLib
    src/projectGenerator.cpp
)

target_include_directories(ProjGenCppLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --- Testy ---
add_executable(ProjGenCpp
    src/main.cpp
)

target_link_libraries(ProjGenCpp
    PRIVATE ProjGenCppLib
)


# Konsola (MinGW / Windows)
if(MINGW)
    target_link_options(ProjGenCpp PRIVATE -mconsole)

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/templates
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/templates
    )
endif()