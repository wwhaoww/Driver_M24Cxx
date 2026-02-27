#include "Int_EEPROM24xx.h"

/*
 * 移植模板：你只需要实现这 3 个函数。
 */

static IntEEPROM_Result_t your_i2c_read(uint16_t dev_addr,
                                        uint32_t mem_addr,
                                        uint8_t mem_addr_size,
                                        uint8_t *buf,
                                        uint16_t len,
                                        uint32_t timeout_ms)
{
    (void)dev_addr;
    (void)mem_addr;
    (void)mem_addr_size;
    (void)buf;
    (void)len;
    (void)timeout_ms;

    /* TODO: 替换为你的平台 I2C 读实现 */
    return INT_EEPROM_ERR_IO;
}

static IntEEPROM_Result_t your_i2c_write(uint16_t dev_addr,
                                         uint32_t mem_addr,
                                         uint8_t mem_addr_size,
                                         const uint8_t *buf,
                                         uint16_t len,
                                         uint32_t timeout_ms)
{
    (void)dev_addr;
    (void)mem_addr;
    (void)mem_addr_size;
    (void)buf;
    (void)len;
    (void)timeout_ms;

    /* TODO: 替换为你的平台 I2C 写实现 */
    return INT_EEPROM_ERR_IO;
}

static void your_delay_ms(uint32_t delay_ms)
{
    (void)delay_ms;
    /* TODO: 替换为你的平台 delay 实现 */
}

void eeprom_example(void)
{
    IntEEPROM24xx_Config_t cfg = {
        .dev_addr = (0x50u << 1),
        .page_size = 8u,
        .mem_addr_size = 1u,
        .write_cycle_ms = 6u,
        .timeout_ms = 100u,
    };

    uint8_t tx[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    uint8_t rx[8] = {0};

    if (Int_EEPROM24xx_Init(&cfg, your_i2c_read, your_i2c_write, your_delay_ms) != INT_EEPROM_OK)
    {
        return;
    }

    (void)Int_EEPROM24xx_Write(0x00, tx, sizeof(tx));
    (void)Int_EEPROM24xx_Read(0x00, rx, sizeof(rx));
}
