# By setting CMAKE_SYSTEM_NAME to "Generic," you indicate to CMake that it should avoid platform-specific
# configurations and try to generate a more generic build system that can be used across different
# environments. This is often done in cross-compilation scenarios or when building code that is intended to
# be platform-independent.
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_SYSTEM_PROCESSOR cortex-m0plus) 


# Read from cache or set defaults if undefined
set(TARGET "$ENV{TARGET}" CACHE STRING "Fallback target name")
set(EXE_DIR "${CMAKE_BINARY_DIR}/bin" CACHE PATH "Fallback output dir")


# Set the toolchain base path
set(TOOLCHAIN_BASE_PATH "/home/dev/gcc-arm-none-eabi-10.3-2021.10/bin/")

# --- START ADDED SECTION FOR CLANG-TIDY INFO ---

# Derive key toolchain information for use in other CMakeLists.txt files (e.g., clang-tidy setup)

# ARM_TOOLCHAIN_BASE_DIR: This is the parent directory of 'bin/' (e.g., /home/dev/gcc-arm-none-eabi-10.3-2021.10)
get_filename_component(ARM_TOOLCHAIN_ROOT_PATH "${TOOLCHAIN_BASE_PATH}" DIRECTORY)
set(ARM_TOOLCHAIN_BASE_DIR "${ARM_TOOLCHAIN_ROOT_PATH}" CACHE PATH "Base directory of ARM toolchain for clang-tidy")

# ARM_TOOLCHAIN_TRIPLE: The target triple (e.g., arm-none-eabi)
# For 'arm-none-eabi-gcc', the triple is 'arm-none-eabi'.
set(ARM_TOOLCHAIN_TRIPLE "arm-none-eabi" CACHE STRING "Target triple for ARM toolchain (e.g., arm-none-eabi)")

# ARM_GCC_VERSION: The specific version string (e.g., 10.3.2021.10)
# We can extract this from the directory name if it's consistent.
string(REGEX MATCH "gcc-arm-none-eabi-([0-9]+\\.[0-9]+-[0-9]+\\.[0-9]+)" _match "${ARM_TOOLCHAIN_BASE_DIR}")
if(_match)
    set(ARM_GCC_VERSION "${CMAKE_MATCH_1}" CACHE STRING "Version string of GCC compiler in ARM toolchain")
else()
    set(ARM_GCC_VERSION "10.3.2021.10" CACHE STRING "Version string of GCC compiler in ARM toolchain" FORCE) # Fallback if regex fails
    message(WARNING "Could not automatically determine ARM GCC version from path. Using fallback: ${ARM_GCC_VERSION}")
endif()

message(STATUS "Clang-Tidy Toolchain Info:")
message(STATUS "  Base Dir: ${ARM_TOOLCHAIN_BASE_DIR}")
message(STATUS "  Triple:   ${ARM_TOOLCHAIN_TRIPLE}")
message(STATUS "  GCC Ver:  ${ARM_GCC_VERSION}")

# --- END ADDED SECTION ---

# Specify the cross compiler and associated tools
set(CMAKE_C_COMPILER "${TOOLCHAIN_BASE_PATH}arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_BASE_PATH}arm-none-eabi-g++")
set(CMAKE_ASM_COMPILER "${TOOLCHAIN_BASE_PATH}arm-none-eabi-gcc")
set(OBJCOPY_PATH "${TOOLCHAIN_BASE_PATH}arm-none-eabi-objcopy")
set(SIZE_TOOL "${TOOLCHAIN_BASE_PATH}arm-none-eabi-size")

# MCU specific option flags
# We use set to create a list of flags that we want to pass to the compiler. It is a list of strings. 
# This is a convenient and configurable method. The flags we are passing to the compiler include:
#
# -mcpu=cortex-m4:    Specifies the target CPU architecture as Cortex-M4.
# -mthumb:            Indicates that the code should be compiled for the Thumb instruction set, which is
#                     commonly used in ARM-based microcontrollers for code size optimization.
# -mfpu=fpv4-sp-d16:  Specifies the FPU (Floating-Point Unit) type for Cortex-M4, in this case, "fpv4-sp-d16"
#                     stands for a single-precision FPU with 16 double-precision registers.
# -mfloat-abi=hard:   Specifies that the code should use the "hard" floating-point ABI (Application Binary Interface),
#                     which means that floating-point calculations should be performed using hardware instructions
#                     (as opposed to software emulation).
# --specs=nano.specs: This flag is specific to some ARM toolchains (like the GCC ARM toolchain) and is used to
#                     specify linker options for using the Nano Standard C Library, which is a minimalistic
#                     version of the C library optimized for embedded systems with limited resources.
set(MCU_FLAGS
    -mcpu=cortex-m0plus
    -mthumb
    -ffunction-sections 
    -fdata-sections
    -specs=nano.specs
)

# In our configuration with multiple toolchains and languages, CMake struggles a bit processing the
# MCU_FLAGS list. To work around this, we convert the list to a single string with a space delimiter.
string(JOIN " " MCU_FLAGS_STRING "${MCU_FLAGS}")
string(REPLACE ";" " " MCU_FLAGS_STRING "${MCU_FLAGS_STRING}")

# Define additional compiler symbols (-D)
#
add_compile_definitions(
    # __FPU_PRESENT=1U  # removing this because it is defined in the part pack 
)

# Common flags for all compilers
#
# g:                  Generate debug information
# O2:                 Optimize code for speed
# Wall:               Enables a compiler warning messages that catch common programming errors
# Wextra:             Enables some extra warning flags that are not enabled by -Wall
# Werror:             Make all warnings into errors
# pedantic:           Issue all the warnings demanded by strict ISO C and ISO C++
# Wunused-variable:   Warn whenever a local variable or non-constant static variable is unused aside from its
# Wuninitialized:     Warn if an automatic variable is used without first being initialized
# Wshadow:            Warn whenever a local variable or type declaration shadows another variable
# fstack-protector:   Enable stack protection checks
# Wconversion:        Warn for implicit conversions that may alter a value
# Wunused-function:   Warn whenever a static function is declared but not defined or a non-inline static function
# funsigned-char:     Treat character data type as unsigned instead of signed
# fdata-sections:     Instructs the compiler to place each global or static variable in a separate data section
# ffunction-sections: Instructs the compiler to place each function in a separate code section
set(COMMON_FLAGS
    -g
    -O2
    -Wall
#    -Wextra
    # -Werror
#    -pedantic
    -Wunused-variable
    -Wuninitialized
    -Wshadow
    -Wdouble-promotion
    -fstack-protector
#    -Wconversion
    -Wunused-function
    -Wno-unused-parameter
    -funsigned-char
    -fdata-sections
    -ffunction-sections
    -fno-strict-aliasing
    -fno-builtin 
    -fno-common
    -fdebug-prefix-map=${CMAKE_SOURCE_DIR}=.
)


string(REPLACE ";" " " COMMON_FLAGS "${COMMON_FLAGS}")

# C++ flags
#
# std:                Let's use C++17
# fno-rtti:           Disable Run-Time Type Information. Decreases binary size and improves performance
# fno-exceptions:     For microcontrollers, we disable exceptions due to run-time overhead and dynamic allocation
# lstdc++:            C++ should link our application
set(CPP_ONLY_FLAGS
    -std=c++17
    -fno-rtti
    -fno-exceptions
    -lstdc++
)

string(REPLACE ";" " " CPP_ONLY_FLAGS "${CPP_ONLY_FLAGS}")

# C flags
#
# std:                Let's use C11
set(C_ONLY_FLAGS
    -std=c11
)

string(REPLACE ";" " " C_ONLY_FLAGS "${C_ONLY_FLAGS}")

# Set compiler-specific flags
set(CMAKE_CXX_FLAGS "${MCU_FLAGS_STRING} ${COMMON_FLAGS} ${CPP_ONLY_FLAGS}")
set(CMAKE_C_FLAGS "${MCU_FLAGS_STRING} ${COMMON_FLAGS} ${C_ONLY_FLAGS}")
set(CMAKE_ASM_FLAGS "${MCU_FLAGS_STRING} ${COMMON_FLAGS}")

# Link script
set(LDSCRIPT "${CMAKE_SOURCE_DIR}/src/ASF/sam0/utils/linker_scripts/samr30/gcc/samr30g18a_flash.ld")


# Linker options
#
# -T:                 Linker script
# -specs=nosys.specs: Linker options for using the Nano Standard C Library
# -Wl,-Map:           Generate a map file
# -Wl,--cref:         Add cross reference to the map file
# -Wl,--gc-sections:  Remove unused sections from the final binary
set(TOOLCHAIN_LINKER_OPTIONS
    -mthumb
    -mcpu=cortex-m0plus
    -T${LDSCRIPT}
    -Wl,--entry=Reset_Handler
    -Wl,-Map=${EXE_DIR}/${TARGET}.map,--cref
    -Wl,--gc-sections
)

# Libraries to link
#
# c:                  The C library
# m:                  The math library
# nosys:              The Nano Standard C Library
set(TOOLCHAIN_LIBRARIES
    c
    m
    nosys
)

# convert linker flags to a string
string(REPLACE ";" " " TOOLCHAIN_LINKER_FLAGS "${TOOLCHAIN_LINKER_OPTIONS}")
# set cmake linker flags
set(CMAKE_EXE_LINKER_FLAGS "${TOOLCHAIN_LINKER_FLAGS}")

# set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
