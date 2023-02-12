#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "../../src/eeprom.h"

#define I2C_SDA 14
#define I2C_SCL 15

uint8_t addr_eeprom = 0x50;

int main(void) {
    stdio_init_all();
    sleep_ms(3000);
    printf("\n\nhello, this is pico!\n");

    i2c_init(i2c1, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    static uint8_t rbuf[500] = {'0'};
    uint8_t abuf[] = {0x00, 0x00};
    int16_t i;

    int16_t ret = eeprom_read(i2c1, addr_eeprom, 0x0000, rbuf, 500);
    for (i = 1; i <= 500; i++) {
        printf("%2x ", rbuf[i-1]);
        if (i % 10 == 0 && i != 0) printf("\n");
    }
    printf("read: %d bytes, ret: %d\n", i-1, ret);
}
