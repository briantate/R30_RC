# src/ASF/sam0/utils/cmsis/samr30
add_subdirectory(source)

target_include_directories( ${TARGET}
    PRIVATE
        include
        include/component
        include/instance
        include/pio
        source
)