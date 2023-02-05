#include "ep-log.h"

#if 0
// init given i2c bus, and gpio_set_function to "sda", "sda" + 1
void eplog_i2c_init(i2c_inst_t *i2c, uint8_t sda) {
    i2c_init(i2c, 400 * 1000);
    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_set_function(sda + 1, GPIO_FUNC_I2C);
}
#endif

// 上位WORD ADDRESSが自動で切り替わらない対策用
// overflow: 1, not: 0
static uint8_t is_overflow_8(uint8_t regl, uint8_t len) {
    return (((regl & len) | ((regl | len) & ~(regl + len))) >> 7);
}

// ページが自動で切り替わらない対策用
// overflow: 1, not: 0
static uint8_t is_overflow_16(uint16_t reg, uint16_t len) {
    return (((reg & len) | ((reg | len) & ~(reg + len))) >> 15);
}

int16_t eplog_write_reg(i2c_inst_t *i2c, uint8_t addr, uint16_t reg, uint8_t *src, uint8_t len) {
    uint8_t regl = reg & 0xFF;
    int16_t ret = 0;
    
    uint8_t wbuf[] = {(uint8_t)(reg >> 8), regl};
    i2c_write_blocking(i2c, addr, wbuf, 2, true);

    // WORD ADDRESSの上位が自動でインクリメントしない対策
    // 次の上位WORD ADDRESSにまたがる場合
    if (is_overflow_8(regl, len)) {
        // if cond == 1, it means (regl + len) overflow. from "Hacker's Delight"
        uint8_t remain = 0xFF - regl;
        ret = i2c_write_nors_blocking(i2c, addr, src, remain, false);
        reg += len - remain;
        ret += i2c_write_nors_blocking(i2c, addr, &src[len - remain], len - remain, false);
    } else {
        ret = i2c_write_nors_blocking(i2c, addr, src, len, false);
    }

    return ret;
}

// write log in eeprom with auto incrementing register address (maximum 256 bytes each call)
// it is reccomended to write 4 bytes each for longer eeprom lifetime
int16_t eplog_write_inc(i2c_inst_t *i2c, uint8_t device_code, uint8_t *src, uint8_t len) {
    // EEPROM I2C ADDRESS
    // MSB - 1 0 1 0 A2 A1 P0 R/W - LSB
    // Prometheus: A2 = A1 = 0
    // so, page 0: addr = 0x50, page 1: 0x51
    // uint8_t device_code = 0x50;
    static uint16_t reg = 0x0000; // 0x0000 to 0xFFFF
    static uint8_t addr;
    static uint8_t is_first = 1;
    if (is_first) {
        addr = device_code;
        is_first = 0;
    }
    int16_t ret = 0;

    // ページが自動で切り替わらない対策
    if (is_overflow_16(reg, len)) {
        /*
        idea:
        remain = 0xFFFF - reg
        first = remain : page 0
        then = len[remain] : page 1, reg 0x0000
        */
        uint8_t remain = 0xFFFF - reg;
        ret = eplog_write_reg(i2c, addr, reg, src, remain);
        addr = (device_code) ? (device_code | 0x01) : (device_code);
        reg = 0x0000;
        ret = eplog_write_reg(i2c, addr, reg, &src[len - remain], len - remain);
    } else {
        ret = eplog_write_reg(i2c, addr, reg, src, len);
    }

    reg += ((ret < 0) ? 0 : ret);
    return ret; // bytes written
}

int16_t eplog_read_reg(i2c_inst_t *i2c, uint8_t addr, uint16_t reg, uint8_t* dst, uint16_t len) {
    uint8_t tmp[2];

    // polling
    uint8_t i = 0;
    while(1) {
        uint8_t ret = i2c_read_blocking(i2c1, addr, &tmp[0], 1, false);
        if (ret > 0) break;
        sleep_ms(1);
        i++;
    }
#ifndef NDEBUG
    printf("wait: %d ms\n", i);
#endif

    // sequential read
    tmp[0] = (uint8_t)(reg >> 8); tmp[1] = (uint8_t)(reg & 0xFF);
    i2c_write_blocking(i2c1, addr, tmp, 2, true);
    int8_t ret = i2c_read_blocking(i2c1, addr, dst, len, false);

    return ret;
}

// read log in eeprom with auto incrementing register address (theoretically max 65535 bytes each call)
int16_t eplog_read_inc(i2c_inst_t *i2c, uint8_t device_code, uint8_t *dst, uint16_t len) {
    static uint16_t reg = 0x0000;
    static uint8_t addr;
    static uint8_t is_first = 1;
    if (is_first) {
        addr = device_code;
        is_first = 0;
    }
    int16_t ret = 0;

    if (is_overflow_16(reg, len)) {
        uint8_t remain = 0xFFFF - reg;
        ret = eplog_read_reg(i2c, addr, reg, dst, remain);
        addr = (device_code) ? (device_code | 0x01) : (device_code);
        reg = 0x0000;
        ret = eplog_read_reg(i2c, addr, reg, &dst[len - remain], len - remain);
    } else {
        ret = eplog_read_reg(i2c, addr, reg, dst, len);
    }

    reg += ((ret < 0) ? 0 : ret);
    return ret;
}

static void printf_null_lf(uint8_t str[], uint8_t len) {
    for (uint8_t i = 0; i < len; i++) {
        if (str[i] == '\0') printf("\n");
        else printf("%c", str[i]);
    }
}

// read and print all data stored in eeprom, the value of "max" each
// if you want to print ALL data in eeprom:
// you should call this TWICE with "max" = 65536 (2^16)
void eplog_read_print_all(i2c_inst_t *i2c, uint8_t device_code, uint16_t max) {
    uint8_t buf[128];
    for (uint16_t remain = max; remain > 0; remain -= 128) {
        if (remain >= 128) {
            eplog_read_inc(i2c, device_code, buf, 128);
            printf_null_lf(buf, 128);
        } else {
            eplog_read_inc(i2c, device_code, buf, remain);
            printf_null_lf(buf, remain);
        }
    }
}



