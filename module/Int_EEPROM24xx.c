#include "Int_EEPROM24xx.h"

static IntEEPROM24xx_Config_t g_cfg;
static IntEEPROM_I2CRead_t g_read_fn;
static IntEEPROM_I2CWrite_t g_write_fn;
static IntEEPROM_DelayMs_t g_delay_fn;
static uint8_t g_inited;

IntEEPROM_Result_t Int_EEPROM24xx_Init(const IntEEPROM24xx_Config_t *cfg,
                                       IntEEPROM_I2CRead_t read_fn,
                                       IntEEPROM_I2CWrite_t write_fn,
                                       IntEEPROM_DelayMs_t delay_fn)
{
    if (cfg == NULL || read_fn == 0 || write_fn == 0)
    {
        return INT_EEPROM_ERR_ARG;
    }

    if (cfg->page_size == 0u || (cfg->mem_addr_size != 1u && cfg->mem_addr_size != 2u))
    {
        return INT_EEPROM_ERR_ARG;
    }

    g_cfg = *cfg;
    g_read_fn = read_fn;
    g_write_fn = write_fn;
    g_delay_fn = delay_fn;
    g_inited = 1u;
    return INT_EEPROM_OK;
}

/* 直接透传到用户提供的总线读回调。 */
IntEEPROM_Result_t Int_EEPROM24xx_Read(uint32_t mem_addr, uint8_t *buf, uint16_t len)
{
    if (g_inited == 0u || buf == 0 || len == 0u)
    {
        return INT_EEPROM_ERR_ARG;
    }

    return g_read_fn(g_cfg.dev_addr, mem_addr, g_cfg.mem_addr_size, buf, len, g_cfg.timeout_ms);
}

/* 按页拆分写入，避免跨页写导致 EEPROM 数据回卷或写失败。 */
IntEEPROM_Result_t Int_EEPROM24xx_Write(uint32_t mem_addr, const uint8_t *buf, uint16_t len)
{
    uint16_t written = 0;
    uint16_t chunk;
    uint32_t cur_addr;
    uint16_t page_off;
    uint16_t room;
    IntEEPROM_Result_t st;

    if (g_inited == 0u || buf == 0 || len == 0u)
    {
        return INT_EEPROM_ERR_ARG;
    }

    while (written < len)
    {
        /* 计算当前页内偏移和本次最多可写长度。 */
        cur_addr = mem_addr + written;
        page_off = (uint16_t)(cur_addr % g_cfg.page_size);
        room = (uint16_t)(g_cfg.page_size - page_off);
        chunk = (uint16_t)(((len - written) < room) ? (len - written) : room);

        /* 实际总线写由平台适配层完成。 */
        st = g_write_fn(g_cfg.dev_addr,
                        cur_addr,
                        g_cfg.mem_addr_size,
                        (const uint8_t *)(buf + written),
                        chunk,
                        g_cfg.timeout_ms);
        if (st != INT_EEPROM_OK)
        {
            return st;
        }

        if (g_delay_fn != 0 && g_cfg.write_cycle_ms > 0u)
        {
            /* EEPROM 页写后通常需要写周期时间。 */
            g_delay_fn(g_cfg.write_cycle_ms);
        }
        written = (uint16_t)(written + chunk);
    }

    return INT_EEPROM_OK;
}
