#ifndef INT_EEPROM24XX_H
#define INT_EEPROM24XX_H

#include <stdint.h>

typedef int32_t IntEEPROM_Result_t;

#define INT_EEPROM_OK 0
#define INT_EEPROM_ERR_ARG -1
#define INT_EEPROM_ERR_IO -2

typedef IntEEPROM_Result_t (*IntEEPROM_I2CRead_t)(uint16_t dev_addr,
                                                   uint32_t mem_addr,
                                                   uint8_t mem_addr_size,
                                                   uint8_t *buf,
                                                   uint16_t len,
                                                   uint32_t timeout_ms);

typedef IntEEPROM_Result_t (*IntEEPROM_I2CWrite_t)(uint16_t dev_addr,
                                                    uint32_t mem_addr,
                                                    uint8_t mem_addr_size,
                                                    const uint8_t *buf,
                                                    uint16_t len,
                                                    uint32_t timeout_ms);

typedef void (*IntEEPROM_DelayMs_t)(uint32_t delay_ms);

typedef struct
{
    uint16_t dev_addr;
    uint16_t page_size;
    uint8_t mem_addr_size;   /* 1 or 2 */
    uint32_t write_cycle_ms;
    uint32_t timeout_ms;
} IntEEPROM24xx_Config_t;

/* 初始化模块：用户传入 I2C 读、写和毫秒延时函数后，即可直接调用读写 API。 */
IntEEPROM_Result_t Int_EEPROM24xx_Init(const IntEEPROM24xx_Config_t *cfg,
                                       IntEEPROM_I2CRead_t read_fn,
                                       IntEEPROM_I2CWrite_t write_fn,
                                       IntEEPROM_DelayMs_t delay_fn);

IntEEPROM_Result_t Int_EEPROM24xx_Read(uint32_t mem_addr, uint8_t *buf, uint16_t len);
IntEEPROM_Result_t Int_EEPROM24xx_Write(uint32_t mem_addr, const uint8_t *buf, uint16_t len);

#endif
