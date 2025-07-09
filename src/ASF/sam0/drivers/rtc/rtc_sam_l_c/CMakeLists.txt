# src/ASF/sam0/drivers/rtc/rtc_sam_l_c

target_sources( ${PROJECT_NAME}.elf
    PRIVATE
        rtc_count.c
        rtc_count_interrupt.c
)