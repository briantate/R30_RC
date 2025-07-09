# src/ASF/sam0/drivers/sercom/usart

target_sources( ${PROJECT_NAME}.elf
    PRIVATE
        usart.c
        usart_interrupt.c
)