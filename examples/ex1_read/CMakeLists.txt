add_executable(read read.c)

target_link_libraries(read pico_stdlib hardware_i2c eeprom)

pico_enable_stdio_usb(read 1)
pico_enable_stdio_uart(read 0)

pico_add_extra_outputs(read)