#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "../../src/ep-log.h"

#define I2C_SDA 14
#define I2C_SCL 15

uint8_t base_eeprom = 0x50;

int main(void) {
    stdio_init_all();
    sleep_ms(3000);
    printf("\n\nhello, this is pico!\n");

    i2c_init(i2c1, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    uint8_t wbuf[95];
    for (uint8_t i = 0; i < 94; i++) wbuf[i] = 33 + i;
    wbuf[94] = '\0';

    eplog_write_inc(i2c1, base_eeprom, wbuf, 95);
    eplog_read_print_all(i2c1, base_eeprom, 100);

    return 0;
}