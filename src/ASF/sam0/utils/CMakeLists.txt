# src/ASF/sam0/utils
add_subdirectory(cmsis/samr30)
add_subdirectory(stdio)
add_subdirectory(syscalls/gcc)

target_include_directories( ${TARGET}
    PRIVATE
        header_files
        preprocessor
)