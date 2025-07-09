# src/ASF/thirdparty/wireless/miwi/source/mimac
add_subdirectory(phy/at86rf212b)

target_sources( ${PROJECT_NAME}.elf
    PRIVATE
        mimac_at86rf.c
)