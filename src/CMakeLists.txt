add_library(eeprom INTERFACE)

target_sources(eeprom INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/eeprom.c
    ${CMAKE_CURRENT_LIST_DIR}/i2c_write_nors_blocking.c
)

target_link_libraries(eeprom
    INTERFACE
    pico_stdlib
    hardware_i2c
)