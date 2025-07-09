# src/
add_subdirectory(ASF)
# add_subdirectory(config)
add_subdirectory(wireless)

target_include_directories( ${TARGET}
    PRIVATE
        config
)

target_sources( ${TARGET}
    PRIVATE
        analog.c
        app.c
        custom_board.c
        debug_interface.c
        joystick.c
        main.c
        network_management.c
        rf_transceiver.c
)