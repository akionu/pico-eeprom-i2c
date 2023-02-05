# ep-log: Eeprom Prometheus LOG
add_library(ep-log INTERFACE)

target_sources(ep-log INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/ep-log.c
    ${CMAKE_CURRENT_LIST_DIR}/i2c_write_nors_blocking.c
)

target_link_libraries(ep-log
    INTERFACE
    pico_stdlib
    hardware_i2c
)