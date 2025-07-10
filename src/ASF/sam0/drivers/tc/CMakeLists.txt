# src/ASF/sam0/drivers/tc
add_subdirectory(tc_sam_l_c)

target_sources( ${TARGET}
    PRIVATE
        tc_interrupt.c
)