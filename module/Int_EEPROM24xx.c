#include "Int_EEPROM24xx.h"

/* 直接透传到用户提供的总线读回调。 */
IntEEPROM_Result_t Int_EEPROM24xx_Read(const IntEEPROM24xx_t *dev, uint32_t mem_addr, uint8_t *buf, uint16_t len)
{
    if (dev == NULL || buf == NULL || len == 0u || dev->bus_read == NULL)
    {
        return INT_EEPROM_ERR_ARG;
    }

    return dev->bus_read(dev->ctx, dev->dev_addr, mem_addr, dev->mem_addr_size, buf, len, dev->timeout_ms);
}

/* 按页拆分写入，避免跨页写导致 EEPROM 数据回卷或写失败。 */
IntEEPROM_Result_t Int_EEPROM24xx_Write(const IntEEPROM24xx_t *dev, uint32_t mem_addr, const uint8_t *buf, uint16_t len)
{
    uint16_t written = 0;
    uint16_t chunk;
    uint32_t cur_addr;
    uint16_t page_off;
    uint16_t room;
    IntEEPROM_Result_t st;

    if (dev == NULL || buf == NULL || len == 0u || dev->page_size == 0u || dev->bus_write == NULL)
    {
        return INT_EEPROM_ERR_ARG;
    }

    while (written < len)
    {
        /* 计算当前页内偏移和本次最多可写长度。 */
        cur_addr = mem_addr + written;
        page_off = (uint16_t)(cur_addr % dev->page_size);
        room = (uint16_t)(dev->page_size - page_off);
        chunk = (uint16_t)(((len - written) < room) ? (len - written) : room);

        /* 实际总线写由平台适配层完成。 */
        st = dev->bus_write(dev->ctx,
                            dev->dev_addr,
                            cur_addr,
                            dev->mem_addr_size,
                            (const uint8_t *)(buf + written),
                            chunk,
                            dev->timeout_ms);
        if (st != INT_EEPROM_OK)
        {
            return st;
        }

        if (dev->delay_ms != NULL && dev->write_cycle_ms > 0u)
        {
            /* EEPROM 页写后通常需要写周期时间。 */
            dev->delay_ms(dev->ctx, dev->write_cycle_ms);
        }
        written = (uint16_t)(written + chunk);
    }

    return INT_EEPROM_OK;
}
