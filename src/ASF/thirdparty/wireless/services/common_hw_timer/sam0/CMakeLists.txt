# src/ASF/thirdparty/wireless/services/common_hw_timer/sam0
add_subdirectory(lib)

target_sources( ${TARGET}
    PRIVATE
        hw_timer.c
)