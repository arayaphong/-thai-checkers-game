cmake_minimum_required(VERSION 3.14)

# --- Code coverage flags ---
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -O0 -g")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

project(ThaiCheckers)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Option for code coverage
option(CODE_COVERAGE "Enable coverage reporting" ON)
if(CODE_COVERAGE)
    message(STATUS "Building with code coverage flags")
    add_compile_options(--coverage -O0 -g)
    add_link_options(--coverage)
endif()

# Include directories
include_directories(include)

# Source files (excluding main.cpp for library)
set(LIB_SOURCES
    src/Board.cpp
    src/Piece.cpp
    src/GameModel.cpp  # Add new GameModel
)

# Create a library from the source files
add_library(ThaiCheckersLib ${LIB_SOURCES})
target_include_directories(ThaiCheckersLib PUBLIC include)

# Create the executable
add_executable(ThaiCheckers src/main.cpp)
target_link_libraries(ThaiCheckers PRIVATE ThaiCheckersLib)

# Include FetchContent to download GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
# Prevent overriding parent project's compiler/linker flags on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add tests
enable_testing()
add_subdirectory(tests)

# Custom target to automate coverage
add_custom_target(coverage
    COMMAND ${CMAKE_COMMAND} -E rm -rf coverage_html
    COMMAND ${CMAKE_COMMAND} -E make_directory coverage_html/v1
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_BINARY_DIR} ctest
    COMMAND gcovr -r ${CMAKE_SOURCE_DIR} --html --html-details -o coverage_html/v1/index.html --filter include/GameModel.h --filter include/Move.h --filter include/Piece.h --filter include/Position.h --filter src/Board.cpp --filter src/GameModel.cpp --filter src/Piece.cpp
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating code coverage report in coverage_html/v1/index.html (filtered)"
)