# CMakeLists.txt (now located in tests/TestStateMachine/ directory)

cmake_minimum_required(VERSION 3.10)
project(GenericStateMachineTests C) # Renamed project to be more specific to this test suite

# Find CppUTest
# If CppUTest is installed system-wide, CMake might find it automatically.
# Otherwise, you might need to set CPPUTEST_HOME environment variable
# or provide its path in CMake.
find_package(CppUTest CONFIG REQUIRED)

# Define source directories relative to this CMakeLists.txt (which is in 'tests/TestStateMachine/')
set(SRC_ROOT_DIR ../../src) # The original 'src' directory is now two levels up
set(TEST_DIR .)              # The current directory is 'tests/TestStateMachine/'

# Add include directories
include_directories(
    ${SRC_ROOT_DIR}
    ${SRC_ROOT_DIR}/state_machine # This covers StateMachine.h, Event.h, and State.h
    ${TEST_DIR}                  # This covers headers directly in this test directory
    ${CPPUTEST_INCLUDE_DIRS}
)

# Define the source files for the state machine modules being tested (relative to SRC_ROOT_DIR)
set(STATE_MACHINE_SRCS
    ${SRC_ROOT_DIR}/state_machine/state_machine.c
    # DoorFSM.c is an application-specific module and should NOT be included
    # in the generic StateMachine tests for isolation.
    # If you create a tests/TestDoorFSM/ directory, DoorFSM.c would be linked there.
)

# Define the header files (not compiled directly, but good for reference)
set(STATE_MACHINE_HDRS
    ${SRC_ROOT_DIR}/state_machine/event.h
    ${SRC_ROOT_DIR}/state_machine/state.h
    ${SRC_ROOT_DIR}/state_machine/state_machine.h
    ${SRC_ROOT_DIR}/DoorFSM.h # This header is still needed for DoorFSM.c if it were linked here
)

# --- Build the Test Executable for Generic State Machine ---
add_executable(TestStateMachineRunner # Renamed executable for clarity
    ${TEST_DIR}/AllTests.cpp
    ${TEST_DIR}/TestStateMachine.cpp
    ${STATE_MACHINE_SRCS}
)

# Link with CppUTest libraries
target_link_libraries(TestStateMachineRunner
    CppUTest::CppUTest
    CppUTest::CppUTestExt
)

# Add a custom command to run tests
add_test(NAME RunGenericStateMachineTests COMMAND TestStateMachineRunner)

# Optional: Install CppUTest if not already installed (for CI/CD setups)
# add_subdirectory(path/to/CppUTest)
# target_link_libraries(TestStateMachineRunner CppUTest CppUTestExt)
