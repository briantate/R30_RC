# src/ASF/common2/services/delay
add_subdirectory(sam0)

target_include_directories( ${TARGET}
    PRIVATE
        sam0
)