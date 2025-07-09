# src/ASF/sam0/drivers/extint
add_subdirectory(extint_sam_l_c)

target_sources( ${PROJECT_NAME}.elf
    PRIVATE
        extint_callback.c
)