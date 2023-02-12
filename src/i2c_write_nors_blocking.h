#pragma once
#include "pico/stdlib.h"
#include "hardware/i2c.h"

int i2c_write_nors_blocking(i2c_inst_t *i2c, uint8_t addr, const uint8_t *src, size_t len);
