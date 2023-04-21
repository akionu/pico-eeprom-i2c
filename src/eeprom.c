#include "eeprom.h"

#if 0
// overflow: 1, not: 0
static uint8_t is_overflow_8(uint8_t regl, uint8_t len) {
    return (((regl & len) | ((regl | len) & ~(regl + len))) >> 7);
}
#endif

static void eeprom_polling(i2c_inst_t *i2c, uint8_t addr) {
    #if 1
    uint8_t i = 0;
    uint8_t tmp;
    int8_t ret;
    while(1) {
        ret = i2c_read_blocking(i2c, addr, &tmp, 1, false);
        if (ret > 0) break;
        sleep_ms(1);
        i++;
    }
    #endif
    #if 0
    uint8_t i = 20;
    sleep_ms(i);
    #endif
#ifndef NDEBUG
    printf("wait: %d ms\n", i);
#endif
}

int8_t eeprom_write_single(i2c_inst_t *i2c, uint8_t addr, uint16_t reg, uint8_t *src) {
    uint8_t ret;
    uint8_t buf[] = {reg >> 8, reg & 0xFF};
    i2c_write_nors_blocking(i2c, addr, buf, 2);
    ret = i2c_write_nors_blocking(i2c, addr, src, 1);
    return ret;
}

uint16_t eeprom_write_multi(i2c_inst_t *i2c, uint8_t addr, uint16_t reg, uint8_t *src, uint16_t len) {
    int16_t ret;
    uint8_t buf[2];
    uint32_t reg_page_end = reg | 0x00FF;
    uint16_t len_write = len;
    uint16_t pos = 0;
#ifndef NDEBUG
    printf("reg_page_end: 0x%x, len_write: %d, pos: %d\n", reg_page_end, len_write, pos);
#endif
    while (pos < len) {
#ifndef NDEBUG
    printf("reg_page_end: 0x%x, reg: 0x%x, len_write: %d, pos: %d, ret: %d\n", reg_page_end, reg, len_write, pos, ret);
#endif
        if (reg + len_write - 1 > 0xFFFF) reg = 0x0000;
        reg_page_end = reg | 0x00FF;
        if (reg + len_write - 1 > reg_page_end) len_write = reg_page_end - reg + 1;
        buf[0] = reg >> 8; buf[1] = reg & 0xFF;
#ifndef NDEBUG
    printf("reg_page_end: 0x%x, reg: 0x%x, len_write: %d, pos: %d, ret: %d\n", reg_page_end, reg, len_write, pos, ret);
    printf("buf[0]: 0x%x, buf[1]: 0x%x\n", buf[0], buf[1]);
#endif
        i2c_write_blocking(i2c, addr, buf, 2, true);
        ret = i2c_write_nors_blocking(i2c, addr, &src[pos], len_write);
        pos += len_write;
        reg += len_write;
        len_write = len - pos;
        if (ret < 0) break;
        eeprom_polling(i2c, addr);
    }
    return ((ret < 0) ? ret : len);
}

uint16_t eeprom_read(i2c_inst_t *i2c, uint8_t addr, uint16_t reg, uint8_t *dst, uint16_t len) {
    uint16_t ret;
    uint8_t buf[] = {reg >> 8, reg & 0xFF};
    eeprom_polling(i2c, addr);
    i2c_write_blocking(i2c, addr, buf, 2, true);
    ret = i2c_read_blocking(i2c, addr, dst, len, false);
    return ret;
}