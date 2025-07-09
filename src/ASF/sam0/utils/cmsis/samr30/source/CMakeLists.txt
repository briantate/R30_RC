# src/ASF/sam0/utils/cmsis/samr30/source
add_subdirectory(gcc)

target_sources( ${PROJECT_NAME}.elf
    PRIVATE
        system_samr30.c
)