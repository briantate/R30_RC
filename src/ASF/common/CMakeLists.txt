# src/ASF/common
add_subdirectory(utils)

target_include_directories( ${TARGET}
    PRIVATE 
        boards
        services/serial
        services/serial/sam0_usart
        utils  
)