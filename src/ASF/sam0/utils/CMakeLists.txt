# src/ASF/sam0/utils
add_subdirectory(cmsis/samr30/source)
add_subdirectory(stdio)
add_subdirectory(syscalls/gcc)

target_include_directories( ${TARGET}
    PRIVATE
        header_files
        preprocessor
        cmsis/samr30/include
        cmsis/samr30/include/component
        cmsis/samr30/include/instance
        cmsis/samr30/include/pio
        cmsis/samr30/source
)