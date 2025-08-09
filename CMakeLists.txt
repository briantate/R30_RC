cmake_minimum_required(VERSION 3.16.3)
# cmake_policy(SET CMP0079 NEW)  # Allow linking in subdirs

# Define the output directory early for use in toolchain
set(EXE_DIR ${CMAKE_BINARY_DIR}/bin CACHE PATH "Output directory for executables")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXE_DIR})

# Define the project
project(R30_RC C ASM)

# --- Start: clang-tidy Integration additions ---

# 1. Ensure compilation database generation is enabled early.
#    This is crucial for clang-tidy to get the correct build flags (toolchain, defines, includes).
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Generate compile_commands.json for clang-tidy" FORCE)

# 2. Find clang-tidy and run-clang-tidy.py executables.
#    These tools need to be available in your system's PATH.
#    On Debian/Ubuntu, you'd typically install 'clang-tools-extra'.
find_program(PYTHON3_EXECUTABLE NAMES python3 python)
if(NOT PYTHON3_EXECUTABLE)
    message(FATAL_ERROR "Python 3 executable not found. Please ensure python3 is in your PATH.")
endif()

find_program(CLANG_TIDY_EXECUTABLE clang-tidy)
if(NOT CLANG_TIDY_EXECUTABLE)
    message(FATAL_ERROR "clang-tidy executable not found. Please ensure it's in your PATH. On Debian/Ubuntu: sudo apt install clang-tools-extra")
endif()

find_program(RUN_CLANG_TIDY_SCRIPT run-clang-tidy-18.py)
if(NOT RUN_CLANG_TIDY_SCRIPT)
    message(FATAL_ERROR "run-clang-tidy.py script not found. Please ensure clang-tools-extra is installed and in your PATH.")
endif()

set(RUN_CLANG_TIDY_SCRIPT_PATH "/usr/local/bin/run-clang-tidy.py")

# Verify the script exists at the expected path (optional, but good for debugging)
if(NOT EXISTS "${RUN_CLANG_TIDY_SCRIPT_PATH}")
    message(FATAL_ERROR "run-clang-tidy.py script not found at ${RUN_CLANG_TIDY_SCRIPT_PATH}. "
                       "Please check Dockerfile symlink and container path.")
endif()

# --- End: clang-tidy Integration additions ---

# Define the target name (can be anything; not required to match project name)
# set(TARGET car)
set(TARGET RC CACHE INTERNAL "Main target name")

# Include directories
set(INCLUDE_DIRS
  ${CMAKE_SOURCE_DIR}/src
)

# Source files
file(GLOB C_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.c")
set(CPP_SOURCES) # Add any .cpp files if needed
set(ASM_SOURCES) # Add any .s or .S files if needed

# compile definitions that need to be accessable project wide
add_compile_definitions(
  DEBUG
  BOARD=USER_BOARD
  ARM_MATH_CM0PLUS=true
  EXTINT_CALLBACK_MODE=true
  USART_CALLBACK_MODE=true
  SYSTICK_MODE
  SPI_CALLBACK_MODE=true
  TC_ASYNC=true
  PROTOCOL_P2P
  PHY_AT86RF212B
  SAL_TYPE=AT86RF2xx
  ADC_CALLBACK_MODE
  __SAMR30G18A__
)

# Add executable target using custom TARGET name
add_executable(${TARGET}
    ${CPP_SOURCES}
    ${C_SOURCES}
    ${ASM_SOURCES}
)

add_subdirectory(src)

# Set output name depending on build type
if(CMAKE_BUILD_TYPE STREQUAL "Simulation")
    set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME ${TARGET}.sim)
else()
    set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME ${TARGET}.elf)
endif()

# Include directories
target_include_directories(${TARGET} PRIVATE ${INCLUDE_DIRS})

# Path fix for debugger inside Docker
target_compile_options(${TARGET} PRIVATE "-fdebug-prefix-map=${CMAKE_SOURCE_DIR}=/")

# Add .bin and .hex generation (only if not simulation)
if(NOT CMAKE_BUILD_TYPE STREQUAL "Simulation")
    add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ${OBJCOPY_PATH} -O binary ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET}.elf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET}.bin
        COMMAND ${OBJCOPY_PATH} -O ihex   ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET}.elf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET}.hex
        COMMENT "Generating binary and hex files"
    )

    add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ${SIZE_TOOL} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET}.elf
        COMMENT "Printing size of the ELF file"
    )
endif()

# Custom targets (Docker, format, static analysis)
add_custom_target(docker_image
    COMMAND docker build -t cicd-dev .
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(docker_run
    COMMAND docker run --rm -it --privileged -v "${CMAKE_SOURCE_DIR}:/home/app" cicd-dev:latest bash
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(format
    COMMAND clang-format --style=Google -i ${CPP_SOURCES} ${C_SOURCES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(cppcheck
    COMMAND cppcheck --enable=all --inconclusive --std=c11 ${C_SOURCES} # Adjusted for C
    COMMAND cppcheck --enable=all --inconclusive --std=c++17 ${CPP_SOURCES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(
    clang-tidy-check
    COMMAND
        /bin/bash -c "for file in \$(find \"${CMAKE_SOURCE_DIR}/src\" -type f \\( -name \"*.c\" -o -name \"*.cpp\" -o -name \"*.h\" \\) ); do echo \"Checking \$file\"; clang-tidy -p \"${CMAKE_BINARY_DIR}/release\" \"\$file\" --extra-arg=-I\"${ARM_TOOLCHAIN_INCLUDE_PATH}\"; done"
    COMMENT
        "Running clang-tidy on C and C++ files in the 'src' directory (excluding 'src/ASF')"
    VERBATIM
)

# Linker options and libraries (from toolchain)
# target_link_options(${TARGET} PRIVATE ${TOOLCHAIN_LINKER_OPTIONS})
# target_link_libraries(${TARGET} ${TOOLCHAIN_LIBRARIES})
