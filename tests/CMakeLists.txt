set(CMAKE_CXX_STANDARD 20)

# Add test source files
set(TEST_SOURCES
    BoardCoreTests.cpp
    PieceTests.cpp
    BoardDisplayTests.cpp
    GameLogicTests.cpp
    GameScenariosTests.cpp
)

# Create the test executable
add_executable(ThaiCheckersTests ${TEST_SOURCES})

# Link the Google Test libraries and project library
target_link_libraries(ThaiCheckersTests PRIVATE 
    gtest_main
    ThaiCheckersLib
)

# Enable testing
enable_testing()

# Add tests
add_test(NAME BoardTests COMMAND ThaiCheckersTests)
add_test(NAME PieceTests COMMAND ThaiCheckersTests)
add_test(NAME BoardDisplayTests COMMAND ThaiCheckersTests)
add_test(NAME GameModelTests COMMAND ThaiCheckersTests)
add_test(NAME GameScenarioTests COMMAND ThaiCheckersTests)
# Removed StatesTest

# Find gcovr for code coverage
find_program(GCOVR_EXECUTABLE NAMES gcovr)
if(CODE_COVERAGE AND NOT GCOVR_EXECUTABLE)
    message(WARNING "gcovr not found, disabling code coverage")
    set(CODE_COVERAGE OFF CACHE BOOL "Enable coverage reporting" FORCE)
endif()

# Automatically generate coverage report after tests if CODE_COVERAGE is ON
if(CODE_COVERAGE)
    add_custom_command(
        TARGET ThaiCheckersTests POST_BUILD
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target coverage
        COMMENT "Automatically generating code coverage report after tests (CODE_COVERAGE=ON)"
    )
else()
    message(STATUS "Coverage post-build disabled due to CODE_COVERAGE=OFF")
endif()