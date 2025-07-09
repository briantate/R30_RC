# src/ASF/common
add_subdirectory(utils)

target_include_directories( ${PROJECT_NAME}.elf
    PRIVATE
        utils   
)