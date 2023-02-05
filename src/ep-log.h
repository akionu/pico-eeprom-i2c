#pragma once

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "i2c_write_nors_blocking.h"

static void printf_null_lf(uint8_t str[], uint8_t len);
static uint8_t is_overflow_8(uint8_t regl, uint8_t len);
static uint8_t is_overflow_16(uint16_t reg, uint16_t len);

int16_t eplog_write_reg(i2c_inst_t *i2c, uint8_t addr, uint16_t reg, uint8_t *src, uint8_t len);
int16_t eplog_write_inc(i2c_inst_t *i2c, uint8_t device_code, uint8_t *src, uint8_t len);

int16_t eplog_read_reg(i2c_inst_t *i2c, uint8_t addr, uint16_t reg, uint8_t* dst, uint16_t len);
int16_t eplog_read_inc(i2c_inst_t *i2c, uint8_t device_code, uint8_t *dst, uint16_t len);
void eplog_read_print_all(i2c_inst_t *i2c, uint8_t device_code, uint16_t max);
