# src/ASF/sam0/utils/stdio

target_sources( ${TARGET}
    PRIVATE
        read.c
        write.c
)

target_include_directories( ${TARGET}
    PRIVATE
        stdio_serial
)