#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "../../src/eeprom.h"

#define I2C_SDA 14
#define I2C_SCL 15

uint8_t eeprom_addr = 0x50;

int main(void) {
    // init
    stdio_init_all();
    sleep_ms(2000);
    printf("\n\nhello, this is simple_log!\n");

    // init i2c bus
    i2c_init(i2c1, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    static uint8_t wbuf[300] = {0};
    static uint8_t rbuf[300] = {'0'};
    int16_t ret;

    // prepare data to write(for demo)
    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 94; j++) wbuf[i*94 + j] = 33 + j;
        wbuf[i*94 + 94] = '\n';
    }
    printf("write:\n");
    for (uint16_t i = 0; i < 94*3+3; i++) printf("%c", wbuf[i]);
    printf("\n");

    // write to eeprom
    ret = eeprom_write_multi(i2c1, eeprom_addr, 0x0000, wbuf, 94*3+3);
    printf("write ret: %d\n", ret);

    // read
    ret = eeprom_read(i2c1, eeprom_addr, 0x0000, rbuf, 94*3+3);
    printf("read ret: %d\n", ret);
    printf("read:\n");
    for (uint16_t i = 0; i < 300; i++) printf("%c", rbuf[i]);
    printf("\n");

    return 0;
}