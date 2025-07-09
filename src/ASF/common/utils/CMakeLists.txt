# src/ASF/common/utils
add_subdirectory(interrupt)

target_include_directories( ${PROJECT_NAME}.elf
    PRIVATE
        interrupt   
)