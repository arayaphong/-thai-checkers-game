set(CMAKE_CXX_STANDARD 17)

# Add test source files
set(TEST_SOURCES
    BoardTests.cpp
    PieceTests.cpp
    BoardDisplayTests.cpp
    StatesTest.cpp
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
add_test(NAME StatesTest COMMAND ThaiCheckersTests)

# Add coverage flags for test targets
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -O0 -g")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()