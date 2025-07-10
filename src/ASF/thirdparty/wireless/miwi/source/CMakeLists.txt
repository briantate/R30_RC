# src/ASF/thirdparty/wireless/miwi/services
add_subdirectory(mimac)
add_subdirectory(miwi_p2p_star)
add_subdirectory(sys)

target_include_directories( ${TARGET}
    PUBLIC
        sys
        mimac
        miwi_p2p_star
)