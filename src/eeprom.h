#pragma once

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "i2c_write_nors_blocking.h"

/**
 * @brief write single byte to eeprom
 * 
 * @param i2c i2c bus
 * @param addr 7bit address of eeprom
 * @param reg register address of eeprom, specify address to be write in
 * @param src data to be written
 * @return int8_t if success return 1, if fails return PICO_ERROR_GENERIC
 */
int8_t   eeprom_write_single(i2c_inst_t *i2c, uint8_t addr, uint16_t reg, uint8_t *src);

/**
 * @brief write multi byte to eeprom
 * 
 * @details auto increment register address over pages
 * @param i2c i2c bus
 * @param addr 7bit address of eeprom
 * @param reg specify register to start writing at
 * @param src data to be written
 * @param len number of bytes to be written
 * @return uint16_t if success return number of bytes written, if fails return PICO_ERROR_GENERIC
 */
uint16_t eeprom_write_multi (i2c_inst_t *i2c, uint8_t addr, uint16_t reg, uint8_t *src, uint16_t len);

/**
 * @brief read single or multi bytes to eeprom
 * 
 * @param i2c i2c bus
 * @param addr 7bit address of eeprom
 * @param reg specify register to start reading at 
 * @param dst buffer to store read bytes in
 * @param len number of bytes to read
 * @return uint16_t if success return number of bytes read, if fails return PICO_ERROR_GENERIC
 */
uint16_t eeprom_read        (i2c_inst_t *i2c, uint8_t addr, uint16_t reg, uint8_t *dst, uint16_t len);

// internal functions
//static uint8_t is_overflow_8(uint8_t regl, uint8_t len);
// wait until writing finish
static void eeprom_polling  (i2c_inst_t *i2c, uint8_t addr);
