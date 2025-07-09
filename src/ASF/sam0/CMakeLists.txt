# src/ASF/sam0
add_subdirectory(drivers)
add_subdirectory(utils)

target_include_directories( ${PROJECT_NAME}.elf
    PUBLIC
        drivers
        utils
)