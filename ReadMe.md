# pico-eeprom-i2c

A Library to Write and Read I2C EEPROMs with Raspberry Pi Pico C/C++ SDK. Kept as simple as possible.

## Usage

1. Integrate this to your project by copy or as git submodule.
1. Set "Page Size"(BYTES_PER_PAGE) and "Memory Size"(MAX_SIZE_BYTES) in [eeprom.h](./src/eeprom.c).
1. See [examples](./examples/) and description below.

## Functions

- eeprom_write_multi(i2c_inst_t *i2c, uint8_t addr, uint16_t reg, uint8_t *src, uint16_t len)  

    @brief write multi byte to eeprom  
    @details auto increment register address over pages  
    @param i2c i2c bus  
    @param addr 7bit address of eeprom  
    @param reg specify register to start writing at  
    @param src data to be written  
    @param len number of bytes to be written  
    @return uint16_t if success return number of bytes written, if fails return PICO_ERROR_GENERIC  

- eeprom_write_single(i2c_inst_t *i2c, uint8_t addr, uint16_t reg, uint8_t *src)

    @brief write single byte to eeprom  
    @param i2c i2c bus  
    @param addr 7bit address of eeprom  
    @param reg register address of eeprom, specify address to be write in  
    @param src data to be written @return int8_t if success return 1, if fails return PICO_ERROR_GENERIC

- eeprom_read(i2c_inst_t *i2c, uint8_t addr, uint16_t reg, uint8_t *dst, uint16_t len)  

    @brief read single or multi bytes to eeprom  
    @param i2c i2c bus  
    @param addr 7bit address of eeprom  
    @param reg specify register to start reading at  
    @param dst buffer to store read bytes in  
    @param len number of bytes to read  
    @return uint16_t if success return number of bytes read, if fails return PICO_ERROR_GENERIC  

## License
BSD 3-Clause
