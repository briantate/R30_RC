# src/ASF/sam0/drivers/system/clock/clock_samr30
target_sources( ${PROJECT_NAME}.elf
    PRIVATE
        clock.c
        gclk.c
)