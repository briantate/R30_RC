cmake_minimum_required(VERSION 3.16.3)
# cmake_policy(SET CMP0079 NEW)  # Allow linking in subdirs

# Define the output directory early for use in toolchain
set(EXE_DIR ${CMAKE_BINARY_DIR}/bin CACHE PATH "Output directory for executables")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXE_DIR})

# Define the project
project(R30_RC C ASM)

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
  NDEBUG
  RTC_COUNT_ASYNC=true
  ARM_MATH_CM0PLUS=true
  PDS_ENABLE_WEAR_LEVELING
  SYSTICK_MODE
  __ATSAMR3018A__
  SAL_TYPE=AT86RF2xx
  TC_ASYNC=true
  EXTINT_CALLBACK_MODE=true
  BOARD=USER_BOARD
  USART_CALLBACK_MODE=true
  SPI_CALLBACK_MODE=true
  PHY_AT86RF212B
  PROTOCOL_P2P
  ENABLE_NETWORK_FREEZER
  ADC_CALLBACK_MODE
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
    COMMAND clang-format --style=Google -i ${CPP_SOURCES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(cppcheck
    COMMAND cppcheck --enable=all --inconclusive --std=c++17 ${CPP_SOURCES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Linker options and libraries (from toolchain)
# target_link_options(${TARGET} PRIVATE ${TOOLCHAIN_LINKER_OPTIONS})
# target_link_libraries(${TARGET} ${TOOLCHAIN_LIBRARIES})
