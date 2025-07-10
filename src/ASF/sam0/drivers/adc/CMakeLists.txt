# src/ASF/sam0/drivers/adc
add_subdirectory(adc_sam_l_c)

target_include_directories( ${TARGET}
    PRIVATE
        adc_sam_l_c
)