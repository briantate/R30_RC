# src/ASF/common2/boards/user_board

target_include_directories( ${TARGET}
    PRIVATE
        services/delay
)

target_sources( ${TARGET}
    PRIVATE
        init.c
)
