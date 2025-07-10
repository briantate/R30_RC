# src/ASF/sam0/drivers/system
add_subdirectory(clock)
add_subdirectory(interrupt)
add_subdirectory(pinmux)

target_sources( ${TARGET}
    PUBLIC
        system.c
)

target_include_directories( ${TARGET}
    PUBLIC
        clock
        interrupt
        pinmux
        power/power_sam_l
        reset/reset_sam_l
)