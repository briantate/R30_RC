# src/ASF/sam0/drivers/tc
add_subdirectory(tc_sam_l_c)

target_sources( ${PROJECT_NAME}.elf
    PRIVATE
        tc_interrupt.c
)