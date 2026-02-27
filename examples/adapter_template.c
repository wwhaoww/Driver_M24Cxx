#include "Int_EEPROM24xx.h"

/*
 * 这是移植模板：请把这 3 个函数替换为你的平台实现。
 */

static IntEEPROM_Result_t your_bus_read(void *ctx,
                                        uint16_t dev_addr,
                                        uint32_t mem_addr,
                                        uint8_t mem_addr_size,
                                        uint8_t *buf,
                                        uint16_t len,
                                        uint32_t timeout_ms)
{
    (void)ctx;
    (void)dev_addr;
    (void)mem_addr;
    (void)mem_addr_size;
    (void)buf;
    (void)len;
    (void)timeout_ms;

    /* TODO: 替换为平台 I2C 读函数 */
    return INT_EEPROM_ERR_IO;
}

static IntEEPROM_Result_t your_bus_write(void *ctx,
                                         uint16_t dev_addr,
                                         uint32_t mem_addr,
                                         uint8_t mem_addr_size,
                                         const uint8_t *buf,
                                         uint16_t len,
                                         uint32_t timeout_ms)
{
    (void)ctx;
    (void)dev_addr;
    (void)mem_addr;
    (void)mem_addr_size;
    (void)buf;
    (void)len;
    (void)timeout_ms;

    /* TODO: 替换为平台 I2C 写函数 */
    return INT_EEPROM_ERR_IO;
}

static void your_delay_ms(void *ctx, uint32_t delay_ms)
{
    (void)ctx;
    (void)delay_ms;

    /* TODO: 替换为平台 delay 实现 */
}

/* 示例：设备描述 */
static const IntEEPROM24xx_t g_eeprom = {
    .ctx = 0,
    .dev_addr = (0x50u << 1),
    .page_size = 8u,
    .mem_addr_size = 1u,
    .write_cycle_ms = 6u,
    .timeout_ms = 100u,
    .bus_read = your_bus_read,
    .bus_write = your_bus_write,
    .delay_ms = your_delay_ms,
};

void eeprom_example(void)
{
    uint8_t tx[8] = {1,2,3,4,5,6,7,8};
    uint8_t rx[8] = {0};

    (void)Int_EEPROM24xx_Write(&g_eeprom, 0x00, tx, sizeof(tx));
    (void)Int_EEPROM24xx_Read(&g_eeprom, 0x00, rx, sizeof(rx));
}
