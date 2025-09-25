# src/ASF/sam0

add_subdirectory(drivers)
add_subdirectory(utils)
# add_subdirectory(boards/samr30_xplained_pro)

target_include_directories( ${TARGET}
    PRIVATE
        drivers
        utils
        # boards/samr30_xplained_pro
)