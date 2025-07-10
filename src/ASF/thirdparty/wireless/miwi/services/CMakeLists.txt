# src/ASF/thirdparty/wireless/miwi/services
add_subdirectory(pds/src)
add_subdirectory(sleep_mgr/sam0)

target_include_directories( ${TARGET}
    PRIVATE
        pds/inc
        pds/inc/nv
        pds/inc/wl
        sleep_mgr

)