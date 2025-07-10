# src/ASF/sam0/drivers/sercom
add_subdirectory(spi)
add_subdirectory(usart)

target_sources( ${TARGET}
    PRIVATE
        sercom_interrupt.c
        sercom.c
)

target_include_directories( ${TARGET}
    PRIVATE
        spi
        usart
)