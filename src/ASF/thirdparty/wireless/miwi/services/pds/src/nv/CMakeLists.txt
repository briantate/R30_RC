# src/ASF/thirdparty/wireless/miwi/services/pds/src/nv
add_subdirectory(External)

target_sources( ${PROJECT_NAME}.elf
    PRIVATE
        D_Nv.c
        S_Nv-SamR21.c
)