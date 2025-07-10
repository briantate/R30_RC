# src/ASF/thirdparty/wireless/services/common_hw_timer/sam0/lib
# target_link_libraries( ${TARGET}
#     PRIVATE
#         libsam0_lib_hw_timer.a
# )

add_library(hw_timer_lib STATIC IMPORTED)
set_target_properties(hw_timer_lib PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libsam0_lib_hw_timer.a"
)

target_link_libraries(${TARGET}
    PRIVATE hw_timer_lib
)