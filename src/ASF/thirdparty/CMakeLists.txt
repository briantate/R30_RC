# src/ASF/thirdparty
add_subdirectory(CMSIS/Lib/GCC)
add_subdirectory(wireless)

target_include_directories( ${TARGET}
    PUBLIC
        CMSIS/Include
)