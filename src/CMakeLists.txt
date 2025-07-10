# src/
add_subdirectory(ASF)
# add_subdirectory(config)

target_include_directories( ${TARGET}
    PRIVATE
        config
)

target_sources( ${TARGET}
    PRIVATE
        main.c
        analog.c
        app.c
        custom_board.c
        debug_interface.c
        joystick.c
        network_management.c
        rf_transceiver.c
)