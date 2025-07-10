# src/ASF/thirdparty/wireless/services
add_subdirectory(common_hw_timer)
add_subdirectory(nvm/sam0)
add_subdirectory(sal)
add_subdirectory(trx_access)

target_include_directories( ${TARGET}
    PRIVATE
        common_hw_timer
        nvm
        trx_access
)