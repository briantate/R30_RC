# src/ASF/sam0/drivers
add_subdirectory(adc)
add_subdirectory(extint)
add_subdirectory(nvm)
add_subdirectory(port)
add_subdirectory(rtc/rtc_sam_l_c)
add_subdirectory(sercom)
add_subdirectory(system)
add_subdirectory(tc)

target_include_directories( ${TARGET}
    PRIVATE
        adc
        extint
        nvm
        port
        rtc
        sercom
        system
        tc
)