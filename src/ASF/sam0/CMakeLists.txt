# src/ASF/sam0
add_subdirectory(drivers)
add_subdirectory(utils)

target_include_directories( ${TARGET}
    PRIVATE
        drivers
        utils
)