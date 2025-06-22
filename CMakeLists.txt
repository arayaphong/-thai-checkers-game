cmake_minimum_required(VERSION 3.14)

project(ThaiCheckers)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Source files (excluding main.cpp for library)
set(LIB_SOURCES
    src/Board.cpp
    src/Piece.cpp
    src/KingPiece.cpp
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