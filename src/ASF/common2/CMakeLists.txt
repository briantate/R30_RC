# src/ASF/common2
add_subdirectory(services/delay)
add_subdirectory(boards/user_board)

target_include_directories( ${TARGET}
    PRIVATE
        services/delay
        boards/user_board
)
