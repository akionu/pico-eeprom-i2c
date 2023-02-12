add_executable(simple_log simple_log.c)

target_link_libraries(simple_log pico_stdlib hardware_i2c eeprom)

pico_enable_stdio_usb(simple_log 1)
pico_enable_stdio_uart(simple_log 0)

pico_add_extra_outputs(simple_log)