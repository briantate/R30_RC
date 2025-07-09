# src/ASF/sam0/drivers/sercom
add_subdirectory(spi)
add_subdirectory(usart)

target_sources( ${PROJECT_NAME}.elf
    PRIVATE
        sercom_interrupt.c
        sercom.c
)