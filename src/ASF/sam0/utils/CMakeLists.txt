# src/ASF/sam0/utils
add_subdirectory(cmsis/samr30/source)
add_subdirectory(stdio)
add_subdirectory(syscalls/gcc)

target_include_directories( ${PROJECT_NAME}.elf
    PRIVATE
        header_files
        preprocessor
)