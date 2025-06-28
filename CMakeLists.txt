cmake_minimum_required(VERSION 3.14)

project(ThaiCheckers)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# If no build type is specified, default to Release for O3 optimization
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type (Debug/Release)" FORCE)
endif()

# Add O3 flags for Release configuration
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Add debug flags for Debug configuration
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")

# Option for code coverage
option(CODE_COVERAGE "Enable coverage reporting" ON)
# Find gcovr executable for coverage reporting
find_program(GCOVR_EXECUTABLE NAMES gcovr)
# Disable coverage if gcovr is missing
if(CODE_COVERAGE AND NOT GCOVR_EXECUTABLE)
    message(WARNING "gcovr not found, disabling code coverage")
    set(CODE_COVERAGE OFF CACHE BOOL "Enable coverage reporting" FORCE)
endif()
# Apply coverage flags if enabled
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

# Custom target to automate coverage (only if gcovr was found)
if(CODE_COVERAGE AND GCOVR_EXECUTABLE)
    add_custom_target(coverage
        COMMAND ${CMAKE_COMMAND} -E rm -rf coverage_html
        COMMAND ${CMAKE_COMMAND} -E make_directory coverage_html/v1
        COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_BINARY_DIR} ctest
        COMMAND ${GCOVR_EXECUTABLE} -r ${CMAKE_SOURCE_DIR} --html --html-details -o coverage_html/v1/index.html --filter include/GameModel.h --filter include/Move.h --filter include/Piece.h --filter include/Position.h --filter src/Board.cpp --filter src/GameModel.cpp --filter src/Piece.cpp --exclude src/main.cpp --gcov-ignore-errors=no_working_dir_found
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating code coverage report in coverage_html/v1/index.html (filtered)"
    )
endif()