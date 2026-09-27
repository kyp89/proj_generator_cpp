# ProjGenCpp – generator projektów C++

Narzędzie konsolowe, które tworzy gotowy do zbudowania szkielet projektu C++ opartego o CMake:
strukturę katalogów, `CMakeLists.txt`, konfigurację VS Code (`launch.json`, `tasks.json`),
`.gitignore` oraz przykładowy `main.cpp`. Opcjonalnie dodaje zależności
(SDL3, raylib, JSON, HTTP) pobierane automatycznie przez `FetchContent`.

## Wymagania

- CMake ≥ 3.26
- Kompilator z obsługą C++17 (projekt jest przygotowany pod MinGW-w64 / `MinGW Makefiles`)
- Git (potrzebny do pobierania zależności przez `FetchContent`)
- Dostęp do internetu przy pierwszej konfiguracji wygenerowanego projektu z zależnościami

## Budowanie generatora

```sh
cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

Wersja Release (do osobnego katalogu):

```sh
cmake -B build-release -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build-release
```

Po zbudowaniu katalog `templates/` jest automatycznie kopiowany obok pliku `ProjGenCpp.exe`.
Generator zawsze szuka szablonów w katalogu, w którym leży exe, więc można go uruchamiać
z dowolnego miejsca. Przenosząc exe, trzeba przenieść razem z nim katalog `templates/`.

W VS Code dostępne są konfiguracje uruchomienia **Debug app** oraz **build_release**
(buduje wersję Release do `build-release/` i ją uruchamia).

## Użycie

```sh
ProjGenCpp --projectName <nazwa> [opcje]
```

Uruchomienie bez argumentów albo z `--help` / `-h` wyświetla listę opcji.

| Opcja | Opis | Domyślnie |
|---|---|---|
| `--projectName <nazwa>` | Nazwa projektu (i głównego targetu CMake) | `MyNewProject` |
| `--projectPath <ścieżka>` | Katalog, w którym powstanie folder projektu | bieżący katalog |
| `--projectType <app\|lib>` | `app` – aplikacja, `lib` – biblioteka + program testowy | `app` |
| `--useSDL yes` | Dodaje SDL3 | – |
| `--useRAYLIB yes` | Dodaje raylib | – |
| `--useJSON yes` | Dodaje nlohmann/json | – |
| `--useHTTP yes` | Dodaje cpr (zapytania HTTP/HTTPS) | – |
| `--overwrite yes` | Usuwa istniejący folder projektu i generuje go od nowa | – |
| `--help`, `-h` | Wyświetla pomoc | – |

Bez `--overwrite yes` generator nie nadpisuje istniejącego projektu, tylko wypisuje komunikat
i kończy działanie.

### Przykłady

Zwykła aplikacja:

```sh
ProjGenCpp --projectName MojaAplikacja --projectPath D:/Projekty
```

Gra w raylib:

```sh
ProjGenCpp --projectName MojaGra --useRAYLIB yes
```

Biblioteka korzystająca z JSON i HTTP:

```sh
ProjGenCpp --projectName MojaLib --projectType lib --useJSON yes --useHTTP yes
```

## Wygenerowany projekt

```
<nazwa>/
├── .vscode/
│   ├── launch.json      # debugowanie build/<nazwa>.exe
│   └── tasks.json       # CMake Configure + Build (Debug)
├── include/
├── src/
├── tests/
│   └── main.cpp
├── .gitignore
└── CMakeLists.txt
```

Projekt buduje się poleceniami:

```sh
cmake -B build -G "MinGW Makefiles"
cmake --build build
```

albo w VS Code klawiszem F5 (konfiguracja **Debug app** najpierw buduje projekt).

### Typ `app` (domyślny)

Jeden target – plik wykonywalny `<nazwa>` budowany z `tests/main.cpp`.
Zależności są linkowane do niego jako `PRIVATE`.

### Typ `lib`

- Biblioteka `<nazwa>Lib` budowana z `src/<nazwa>.cpp`, z publicznym katalogiem `include/`.
- Przykładowe pliki `include/<nazwa>.hpp` i `src/<nazwa>.cpp` z funkcją `helloFromLibrary()`.
- Program testowy `<nazwa>` (z `tests/main.cpp`), który linkuje bibliotekę.
- Zależności (SDL, raylib, JSON, HTTP) są linkowane do biblioteki jako `PUBLIC`,
  więc program testowy i każdy inny konsument biblioteki dostaje je automatycznie.

## Dostępne zależności

| Flaga | Biblioteka | Wersja | Target CMake | Nagłówek |
|---|---|---|---|---|
| `--useSDL` | [SDL3](https://github.com/libsdl-org/SDL) | 3.2.14 | `SDL3::SDL3` | `<SDL3/SDL.h>` |
| `--useRAYLIB` | [raylib](https://github.com/raysan5/raylib) | 5.5 | `raylib` | `<raylib.h>` |
| `--useJSON` | [nlohmann/json](https://github.com/nlohmann/json) | 3.12.0 | `nlohmann_json::nlohmann_json` | `<nlohmann/json.hpp>` |
| `--useHTTP` | [cpr](https://github.com/libcpr/cpr) | 1.11.2 | `cpr::cpr` | `<cpr/cpr.h>` |

Uwagi:

- **SDL3** jest budowane jako biblioteka współdzielona; na Windowsie `SDL3.dll` jest po buildzie
  kopiowane obok exe.
- **SDL / raylib** – przy tych flagach `tests/main.cpp` zawiera przykładowe okno zamiast „Hello world”.
  Gdy podane są obie, użyty zostanie przykład SDL.
- **cpr** buduje libcurl ze źródeł, więc pierwsza konfiguracja i build trwają kilka minut.
  Biblioteki są budowane statycznie (`BUILD_SHARED_LIBS OFF`), a HTTPS działa przez
  Windowsowy SChannel – bez potrzeby instalowania OpenSSL.

Przykład użycia JSON i HTTP:

```cpp
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

cpr::Response r = cpr::Get(cpr::Url{"https://httpbin.org/json"});
if (r.status_code == 200) {
    nlohmann::json j = nlohmann::json::parse(r.text);
    std::cout << j["slideshow"]["title"] << std::endl;
}
```

## Jak działają szablony

Pliki w `templates/` są kopiowane do nowego projektu, a w treści podmieniane są znaczniki:

- `{{PROJECT_NAME}}`, `{{CMAKE_VERSION}}` – nazwa projektu i wersja CMake.
- `#{{...}}` w `templates/CMakeLists.txt` (np. `#{{SDL_FETCH}}`, `#{{LIBRARY_TARGET}}`) –
  miejsca, w które wstawiana jest zawartość fragmentów `*.template.txt`. Gdy dana opcja jest
  wyłączona, znacznik jest usuwany.
- `{{LINK_TARGET}}` / `{{LINK_SCOPE}}` we fragmentach `*-link.template.txt` – target i zakres
  linkowania, zależne od typu projektu (`${PROJECT_NAME} PRIVATE` lub `${PROJECT_NAME}Lib PUBLIC`).

```
templates/
├── CMakeLists.txt, main.cpp, launch.json, tasks.json, .gitignore
├── LIB/      # target biblioteki, szablony .hpp/.cpp i main.cpp dla typu lib
├── SDL/      # fetch, link, kopiowanie DLL, przykładowy main.cpp
├── RAYLIB/   # fetch, link, przykładowy main.cpp
├── JSON/     # fetch, link
└── HTTP/     # fetch, link
```

### Dodawanie nowej biblioteki

1. Utwórz `templates/<NAZWA>/` z plikami `<nazwa>-fetch.template.txt` (`FetchContent_Declare` +
   `FetchContent_MakeAvailable`) oraz `<nazwa>-link.template.txt`
   (`target_link_libraries({{LINK_TARGET}} {{LINK_SCOPE}} <target>)`).
2. Dodaj znaczniki `#{{<NAZWA>_FETCH}}` i `#{{<NAZWA>_TARGET_LINK}}` w `templates/CMakeLists.txt`.
3. W [include/projectGenerator.hpp](include/projectGenerator.hpp) dodaj stałe flagi, klucze
   w `CmakeParamsKeys`, ścieżki w nowej przestrzeni `<NAZWA>TemplateFiles` i pole w `ProjectParams`.
4. W [src/projectGenerator.cpp](src/projectGenerator.cpp) obsłuż flagę w `getParamsFromArgs`,
   uzupełnij `updateCmakeListsFile` i opis w `printHelp`.
