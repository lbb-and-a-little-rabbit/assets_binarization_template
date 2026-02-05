cmake_minimum_required(VERSION 3.20)
project(assets_binarization_template LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# find SFML
find_package(SFML 3 REQUIRED COMPONENTS System Window Graphics Audio)

# find Python interpreter to run generator script
find_package(Python3 COMPONENTS Interpreter REQUIRED)

# where assets live
set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)

# generated files will go into build/generated
set(GEN_DIR ${CMAKE_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GEN_DIR})

# generator script path (we will add tools/embed_assets.py)
set(ASSET_EMBEDDER ${CMAKE_SOURCE_DIR}/tools/embed_assets.py)

# generated C++ and header
set(GENERATED_CPP ${GEN_DIR}/assets_data.cpp)
set(GENERATED_HEADER ${GEN_DIR}/assets_symbols.h)

# custom command: run python embedder to create generated files
add_custom_command(
    OUTPUT ${GENERATED_CPP} ${GENERATED_HEADER}
    COMMAND ${Python3_EXECUTABLE} ${ASSET_EMBEDDER} ${ASSETS_DIR} ${GEN_DIR}
    DEPENDS ${ASSET_EMBEDDER}
    COMMENT "Generating embedded asset C++ from ${ASSETS_DIR}"
    VERBATIM
)

add_custom_target(assets_code ALL DEPENDS ${GENERATED_CPP} ${GENERATED_HEADER})

# ---------- library (game_core) ----------
add_library(game_core SHARED
    src/Game.cpp
    src/Player.cpp
    ${GENERATED_CPP}    # include generated assets_data.cpp into DLL
)

add_dependencies(game_core assets_code)

# include dirs: your include + generated header dir
target_include_directories(game_core PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${GEN_DIR}    # assets_symbols.h lives here
)

target_link_libraries(game_core PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
)

# ---------- executable ----------
add_executable(game
    main.cpp
)

target_link_libraries(game PRIVATE
    game_core
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
)

# output directories
set_target_properties(game PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
set_target_properties(game_core PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
