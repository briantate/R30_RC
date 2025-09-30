# src/
add_subdirectory(ASF)
add_subdirectory(network)
add_subdirectory(state_machine)

target_include_directories( ${TARGET}
    PRIVATE
        config
        network
        state_machine
)

target_sources( ${TARGET}
    PRIVATE
        main.c
        analog.c
        remote_control.c
        custom_board.c
        debug_interface.c
        joystick.c
        rf_transceiver.c
)