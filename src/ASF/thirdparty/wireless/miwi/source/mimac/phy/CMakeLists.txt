# src/ASF/thirdparty/wireless/miwi/source/mimac/phy
add_subdirectory(at86rf212b)

target_include_directories( ${TARGET}
    PUBLIC
        at86rf212b
)