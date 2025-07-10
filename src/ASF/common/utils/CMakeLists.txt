# src/ASF/common/utils
add_subdirectory(interrupt)

target_include_directories( ${TARGET}
    PRIVATE
        interrupt   
)