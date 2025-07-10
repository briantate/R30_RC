# src/ASF/common2
add_subdirectory(services/delay)

target_include_directories( ${TARGET}
    PRIVATE
        services/delay
)
