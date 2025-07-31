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

# add_custom_target(clang-tidy-check
#     COMMENT "Running clang-tidy checks..."
#     # Run the run-clang-tidy.py script, pointing it to the compilation database.
#     #    The script will automatically pick up the .clang-tidy configuration file
#     #    if placed in your project's root directory.
#     COMMAND ${RUN_CLANG_TIDY_SCRIPT}
#             -p=${CMAKE_BINARY_DIR} # Specify the path to the compilation database
#             WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} # Run from source dir where .clang-tidy typically resides
#     USES_TERMINAL # If you want output to be directly visible in the terminal
# )

# add_custom_target(
#     clang-tidy-check
#     COMMENT "Running clang-tidy checks..."
#     COMMAND ${RUN_CLANG_TIDY_SCRIPT_PATH}
#             -p=${CMAKE_BINARY_DIR} # Path to compilation database
#             -- # Pass subsequent args to clang-tidy's compiler front-end
#             -target=${ARM_TOOLCHAIN_TRIPLE}
#             --sysroot="${ARM_TOOLCHAIN_BASE_DIR}/${ARM_TOOLCHAIN_TRIPLE}"
#             # System include paths for the toolchain
#             -isystem "${ARM_TOOLCHAIN_BASE_DIR}/lib/gcc/${ARM_TOOLCHAIN_TRIPLE}/${ARM_GCC_VERSION}/include"
#             -isystem "${ARM_TOOLCHAIN_BASE_DIR}/lib/gcc/${ARM_TOOLCHAIN_TRIPLE}/${ARM_GCC_VERSION}/include-fixed"
#             -isystem "${ARM_TOOLCHAIN_BASE_DIR}/${ARM_TOOLCHAIN_TRIPLE}/include"
#             # Add any other project-specific include paths that aren't in compile_commands.json but are needed for clang-tidy
#     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#     USES_TERMINAL
# )
# In your main CMakeLists.txt, just before the custom target

# In your main CMakeLists.txt, update the custom target to be more focused.

# Build the clang-tidy command arguments in a variable for clarity and robustness
set(CLANG_TIDY_COMMAND_ARGS
    -p=${CMAKE_BINARY_DIR}
    --
    -target=${ARM_TOOLCHAIN_TRIPLE}
    --sysroot="${ARM_TOOLCHAIN_BASE_DIR}/${ARM_TOOLCHAIN_TRIPLE}"
    -isystem "${ARM_TOOLCHAIN_BASE_DIR}/lib/gcc/${ARM_TOOLCHAIN_TRIPLE}/${ARM_GCC_VERSION}/include"
    -isystem "${ARM_TOOLCHAIN_BASE_DIR}/lib/gcc/${ARM_TOOLCHAIN_TRIPLE}/${ARM_GCC_VERSION}/include-fixed"
    -isystem "${ARM_TOOLCHAIN_BASE_DIR}/${ARM_TOOLCHAIN_TRIPLE}/include"
)

add_custom_target(
    clang-tidy-check
    COMMENT "Running clang-tidy checks..."
    # The build and compile_commands.json generation are handled by the shell script.
    # This command simply runs clang-tidy.
    COMMAND ${PYTHON3_EXECUTABLE} ${RUN_CLANG_TIDY_SCRIPT_PATH} ${CLANG_TIDY_COMMAND_ARGS}

    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    USES_TERMINAL
)

# Linker options and libraries (from toolchain)
# target_link_options(${TARGET} PRIVATE ${TOOLCHAIN_LINKER_OPTIONS})
# target_link_libraries(${TARGET} ${TOOLCHAIN_LIBRARIES})
