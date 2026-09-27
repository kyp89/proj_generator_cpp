cmake_minimum_required(VERSION {{CMAKE_VERSION}})
project({{PROJECT_NAME}} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Wymuszamy tryb Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

#{{FETCH_CONTENT}}
#{{SDL_FETCH}}
#{{RAYLIB_FETCH}}
#{{JSON_FETCH}}
#{{HTTP_FETCH}}

#{{LIBRARY_TARGET}}

# --- Testy ---
add_executable({{PROJECT_NAME}}
    tests/main.cpp
)

#{{LIBRARY_LINK}}
#{{SDL_TARGET_LINK}}
#{{RAYLIB_TARGET_LINK}}
#{{JSON_TARGET_LINK}}
#{{HTTP_TARGET_LINK}}

# Konsola (MinGW / Windows)
if(MINGW)
    target_link_options({{PROJECT_NAME}} PRIVATE -mconsole)
endif()

#{{SDL_COPY_DLL}}