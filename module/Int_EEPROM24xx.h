#ifndef INT_EEPROM24XX_H
#define INT_EEPROM24XX_H

#include <stdint.h>
#include <stddef.h>

typedef int32_t IntEEPROM_Result_t;

#define INT_EEPROM_OK 0
#define INT_EEPROM_ERR_ARG -1
#define INT_EEPROM_ERR_IO -2

/* 底层总线读回调：
 * ctx            : 用户上下文（可传 I2C 句柄、驱动实例等）
 * dev_addr       : 器件地址（格式由你的平台驱动决定）
 * mem_addr       : EEPROM 内存地址
 * mem_addr_size  : 地址字节数（1 或 2）
 * timeout_ms     : 访问超时（毫秒）
 */
typedef IntEEPROM_Result_t (*IntEEPROM_BusRead_t)(void *ctx,
                                                  uint16_t dev_addr,
                                                  uint32_t mem_addr,
                                                  uint8_t mem_addr_size,
                                                  uint8_t *buf,
                                                  uint16_t len,
                                                  uint32_t timeout_ms);

/* 底层总线写回调，参数含义同读回调 */
typedef IntEEPROM_Result_t (*IntEEPROM_BusWrite_t)(void *ctx,
                                                   uint16_t dev_addr,
                                                   uint32_t mem_addr,
                                                   uint8_t mem_addr_size,
                                                   const uint8_t *buf,
                                                   uint16_t len,
                                                   uint32_t timeout_ms);

typedef void (*IntEEPROM_DelayMs_t)(void *ctx, uint32_t delay_ms);

typedef struct
{
    void *ctx;               /* 用户上下文 */
    uint16_t dev_addr;       /* 器件地址 */
    uint16_t page_size;      /* 页大小（字节） */
    uint8_t mem_addr_size;   /* 地址字节数：1 或 2 */
    uint32_t write_cycle_ms; /* 每次页写后等待时间（ms） */
    uint32_t timeout_ms;     /* 总线访问超时（ms） */
    IntEEPROM_BusRead_t bus_read;
    IntEEPROM_BusWrite_t bus_write;
    IntEEPROM_DelayMs_t delay_ms;
} IntEEPROM24xx_t;

/* 读取 EEPROM。
 * 返回 INT_EEPROM_OK 表示成功。
 */
IntEEPROM_Result_t Int_EEPROM24xx_Read(const IntEEPROM24xx_t *dev, uint32_t mem_addr, uint8_t *buf, uint16_t len);

/* 写入 EEPROM（内部会自动按页拆分写入）。 */
IntEEPROM_Result_t Int_EEPROM24xx_Write(const IntEEPROM24xx_t *dev, uint32_t mem_addr, const uint8_t *buf, uint16_t len);

#endif
