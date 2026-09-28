# ProjGenCpp – C++ project generator

A command-line tool that creates a ready-to-build CMake-based C++ project skeleton:
directory structure, `CMakeLists.txt`, VS Code configuration (`launch.json`, `tasks.json`),
`.gitignore` and a sample `main.cpp`. Optionally it adds dependencies
(SDL3, raylib, JSON, HTTP) that are downloaded automatically via `FetchContent`.

## Requirements

- CMake ≥ 3.26
- A C++17 compiler (the project is set up for MinGW-w64 / `MinGW Makefiles`)
- Git (needed by `FetchContent` to download dependencies)
- Internet access on the first configure of a generated project with dependencies

## Building the generator

```sh
cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

Release build (into a separate directory):

```sh
cmake -B build-release -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build-release
```

After the build, the `templates/` directory is copied next to `ProjGenCpp.exe` automatically.
The generator always looks for templates in the directory containing the exe, so it can be run
from anywhere. If you move the exe, move the `templates/` directory along with it.

In VS Code there are two launch configurations: **Debug app** and **build_release**
(builds the Release version into `build-release/` and runs it).

## Usage

```sh
ProjGenCpp --projectName <name> [options]
```

Running without arguments, or with `--help` / `-h`, prints the list of options.

| Option | Description | Default |
|---|---|---|
| `--projectName <name>` | Project name (also the main CMake target) | `MyNewProject` |
| `--projectPath <path>` | Directory in which the project folder is created | current directory |
| `--projectType <app\|lib>` | `app` – application, `lib` – library + test executable | `app` |
| `--useSDL yes` | Add SDL3 | – |
| `--useRAYLIB yes` | Add raylib | – |
| `--useJSON yes` | Add nlohmann/json | – |
| `--useHTTP yes` | Add cpr (HTTP/HTTPS requests) | – |
| `--overwrite yes` | Remove the existing project folder and generate it again | – |
| `--help`, `-h` | Show help | – |

Without `--overwrite yes` the generator does not touch an existing project; it prints a message
and exits.

### Examples

Plain application:

```sh
ProjGenCpp --projectName MyApp --projectPath D:/Projects
```

raylib game:

```sh
ProjGenCpp --projectName MyGame --useRAYLIB yes
```

Library using JSON and HTTP:

```sh
ProjGenCpp --projectName MyLib --projectType lib --useJSON yes --useHTTP yes
```

## Generated project

```
<name>/
├── .vscode/
│   ├── launch.json      # debugs build/<name>.exe
│   └── tasks.json       # CMake Configure + Build (Debug)
├── include/
├── src/
├── tests/
│   └── main.cpp
├── .gitignore
└── CMakeLists.txt
```

Build it with:

```sh
cmake -B build -G "MinGW Makefiles"
cmake --build build
```

or press F5 in VS Code (the **Debug app** configuration builds the project first).

### `app` type (default)

A single target: the `<name>` executable built from `tests/main.cpp`.
Dependencies are linked to it as `PRIVATE`.

### `lib` type

- A `<name>Lib` library built from `src/<name>.cpp`, with `include/` as a public include directory.
- Sample `include/<name>.hpp` and `src/<name>.cpp` files with a `helloFromLibrary()` function.
- A `<name>` test executable (from `tests/main.cpp`) that links the library.
- Dependencies (SDL, raylib, JSON, HTTP) are linked to the library as `PUBLIC`,
  so the test executable and any other consumer of the library get them automatically.

## Available dependencies

| Flag | Library | Version | CMake target | Header |
|---|---|---|---|---|
| `--useSDL` | [SDL3](https://github.com/libsdl-org/SDL) | 3.2.14 | `SDL3::SDL3` | `<SDL3/SDL.h>` |
| `--useRAYLIB` | [raylib](https://github.com/raysan5/raylib) | 5.5 | `raylib` | `<raylib.h>` |
| `--useJSON` | [nlohmann/json](https://github.com/nlohmann/json) | 3.12.0 | `nlohmann_json::nlohmann_json` | `<nlohmann/json.hpp>` |
| `--useHTTP` | [cpr](https://github.com/libcpr/cpr) | 1.11.2 | `cpr::cpr` | `<cpr/cpr.h>` |

Notes:

- **SDL3** is built as a shared library; on Windows `SDL3.dll` is copied next to the exe after
  the build.
- **SDL / raylib** – with these flags `tests/main.cpp` contains a sample window instead of
  "Hello world". If both are given, the SDL sample is used.
- **cpr** builds libcurl from source, so the first configure and build take a few minutes.
  The libraries are built statically (`BUILD_SHARED_LIBS OFF`), and HTTPS works through Windows
  SChannel, so there is no need to install OpenSSL.

JSON + HTTP example:

```cpp
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

cpr::Response r = cpr::Get(cpr::Url{"https://httpbin.org/json"});
if (r.status_code == 200) {
    nlohmann::json j = nlohmann::json::parse(r.text);
    std::cout << j["slideshow"]["title"] << std::endl;
}
```

## How templates work

Files in `templates/` are copied into the new project, and placeholders in their content are
replaced:

- `{{PROJECT_NAME}}`, `{{CMAKE_VERSION}}` – project name and CMake version.
- `#{{...}}` in `templates/CMakeLists.txt` (e.g. `#{{SDL_FETCH}}`, `#{{LIBRARY_TARGET}}`) –
  spots where the content of `*.template.txt` snippets is inserted. When an option is disabled,
  the placeholder is removed.
- `{{LINK_TARGET}}` / `{{LINK_SCOPE}}` in `*-link.template.txt` snippets – the link target and
  scope, depending on the project type (`${PROJECT_NAME} PRIVATE` or `${PROJECT_NAME}Lib PUBLIC`).

```
templates/
├── CMakeLists.txt, main.cpp, launch.json, tasks.json, .gitignore
├── LIB/      # library target, .hpp/.cpp templates and main.cpp for the lib type
├── SDL/      # fetch, link, DLL copy, sample main.cpp
├── RAYLIB/   # fetch, link, sample main.cpp
├── JSON/     # fetch, link
└── HTTP/     # fetch, link
```

### Adding a new library

1. Create `templates/<NAME>/` with `<name>-fetch.template.txt` (`FetchContent_Declare` +
   `FetchContent_MakeAvailable`) and `<name>-link.template.txt`
   (`target_link_libraries({{LINK_TARGET}} {{LINK_SCOPE}} <target>)`).
2. Add the `#{{<NAME>_FETCH}}` and `#{{<NAME>_TARGET_LINK}}` placeholders to
   `templates/CMakeLists.txt`.
3. In [include/projectGenerator.hpp](include/projectGenerator.hpp) add the flag constants,
   keys in `CmakeParamsKeys`, paths in a new `<NAME>TemplateFiles` namespace and a field in
   `ProjectParams`.
4. In [src/projectGenerator.cpp](src/projectGenerator.cpp) handle the flag in
   `getParamsFromArgs`, extend `updateCmakeListsFile` and add the option to `printHelp`.
