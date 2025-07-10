# src/ASF/thirdparty/wireless
add_subdirectory(at86rf2xx/src)

target_include_directories( ${TARGET}
    PUBLIC
        inc
)