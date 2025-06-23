set(CMAKE_CXX_STANDARD 17)

# Add test source files
set(TEST_SOURCES
    BoardTests.cpp
    PieceTests.cpp
    BoardDisplayTests.cpp
    GameModelTests.cpp
    GameScenarioTests.cpp
    # Removed StatesTest.cpp - States class no longer exists
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