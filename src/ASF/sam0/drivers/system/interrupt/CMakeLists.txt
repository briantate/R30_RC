# src/ASF/sam0/drivers/system/interrupt

target_sources( ${TARGET}
    PRIVATE
        system_interrupt.c
)

target_include_directories( ${TARGET}
    PUBLIC
        system_interrupt_samr30
)