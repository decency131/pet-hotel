cmake_minimum_required (VERSION 3.22)

project(ftxui-starter
  LANGUAGES CXX
  VERSION 1.0.0
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- FetchContent setup ------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(FETCHCONTENT_QUIET OFF)

# --- Fetch FTXUI -------------------------------------------------------------
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/arthursonzogni/ftxui.git
  GIT_TAG        v5.0.0
  GIT_PROGRESS   TRUE
  GIT_SHALLOW    TRUE
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(ftxui)
# ------------------------------------------------------------------------------

# --- Fetch nlohmann_json -----------------------------------------------------
FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.12.0      # or another specific version you prefer
  GIT_PROGRESS   TRUE
  GIT_SHALLOW    TRUE
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(json)
# ------------------------------------------------------------------------------

add_executable(ftxui-starter
    src/main.cpp
    include/pethotel.h
    include/animal.h
    include/cat.h
    include/dog.h
    include/rodent.h
    include/kennel.h
    include/reservation.h
    lib/pethotel.cpp
    lib/animal.cpp
    lib/cat.cpp
    lib/dog.cpp
    lib/rodent.cpp
    lib/kennel.cpp
    lib/reservation.cpp

    lib/highlighted_toggle.cpp
    lib/main_menu_page.cpp
    lib/client_choice_page.cpp
    lib/client_page.cpp
    lib/employee_page.cpp
    lib/pet_list_page.cpp
    lib/kennels_page.cpp
    lib/reservations_page.cpp
    )
target_include_directories(ftxui-starter PRIVATE src)

target_link_libraries(ftxui-starter
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
  PRIVATE nlohmann_json::nlohmann_json
)

if (EMSCRIPTEN)
  string(APPEND CMAKE_CXX_FLAGS " -s USE_PTHREADS")
  string(APPEND CMAKE_EXE_LINKER_FLAGS " -s ASYNCIFY")
  string(APPEND CMAKE_EXE_LINKER_FLAGS " -s PROXY_TO_PTHREAD")

  foreach(file "index.html" "run_webassembly.py")
    configure_file("src/${file}" ${file})
  endforeach(file)
endif()
