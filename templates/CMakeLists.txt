cmake_minimum_required(VERSION {{CMAKE_VERSION}})
project({{PROJECT_NAME}} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Wymuszamy tryb Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

#ADD Library
# add_library(ProjGenCppLib
#     src/projectGenerator.cpp
# )

# target_include_directories(ProjGenCppLib
#     PUBLIC
#         ${CMAKE_CURRENT_SOURCE_DIR}/include
# )

# --- Testy ---
add_executable({{PROJECT_NAME}}
    tests/main.cpp
)

# target_link_libraries(ProjGenCpp
#     PRIVATE ProjGenCppLib
# )


# Konsola (MinGW / Windows)
if(MINGW)
    target_link_options({{PROJECT_NAME}} PRIVATE -mconsole)
endif()