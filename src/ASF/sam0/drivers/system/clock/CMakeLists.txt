# src/ASF/sam0/drivers/system/clock
add_subdirectory(clock_samr30)

target_include_directories( ${PROJECT_NAME}.elf
    PRIVATE
        clock_samr30
)