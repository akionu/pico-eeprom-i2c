#pragma once

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "i2c_write_nors_blocking.h"

static uint8_t is_overflow_8(uint8_t regl, uint8_t len);
// wait until writing finish
static void eeprom_polling  (i2c_inst_t *i2c, uint8_t addr);

int8_t   eeprom_write_single(i2c_inst_t *i2c, uint8_t addr, uint16_t reg, uint8_t *src);
int8_t   eeprom_write_page  (i2c_inst_t *i2c, uint8_t addr, uint16_t reg, uint8_t *src, uint16_t len);
uint16_t eeprom_write_multi (i2c_inst_t *i2c, uint8_t addr, uint16_t reg, uint8_t *src, uint16_t len);

uint16_t eeprom_read        (i2c_inst_t *i2c, uint8_t addr, uint16_t reg, uint8_t *dst, uint16_t len);

