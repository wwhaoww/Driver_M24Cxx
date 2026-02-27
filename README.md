# EEPROM24xx 通用驱动模块说明

本 README 对当前 EEPROM 模块代码做统一说明，包含：适用范围、文件职责、移植步骤。

## 1. 适用范围

该模块适用于 I2C 接口的 24xx 系列 EEPROM（如 M24Cxx / AT24Cxx），并支持：

- 1 字节或 2 字节存储地址（`mem_addr_size = 1/2`）
- 页写自动拆分（避免跨页写导致写入异常）
- 平台无关移植（通过用户传入读/写/延时函数）

不适用于：

- 非 I2C EEPROM
- 需要 DMA/异步回调流程的复杂驱动（本模块为同步阻塞接口）

## 2. 文件说明

### `interface/Int_EEPROM24xx.h`

对外头文件，定义：

- 返回码：
  - `INT_EEPROM_OK`
  - `INT_EEPROM_ERR_ARG`
  - `INT_EEPROM_ERR_IO`
- 用户需提供的函数指针类型：
  - `IntEEPROM_I2CRead_t`
  - `IntEEPROM_I2CWrite_t`
  - `IntEEPROM_DelayMs_t`
- 配置结构体 `IntEEPROM24xx_Config_t`
- 对外 API：
  - `Int_EEPROM24xx_Init()`
  - `Int_EEPROM24xx_Read()`
  - `Int_EEPROM24xx_Write()`

### `interface/Int_EEPROM24xx.c`

核心实现文件，主要逻辑：

- `Init`：校验参数、保存配置和用户回调
- `Read`：参数检查后调用用户读函数
- `Write`：按页拆分写入 + 每页写后延时（写周期等待）

## 3. API 使用流程

```c
// 1) 实现底层 3 个函数
static IntEEPROM_Result_t my_i2c_read(uint16_t dev_addr,
                                      uint32_t mem_addr,
                                      uint8_t mem_addr_size,
                                      uint8_t *buf,
                                      uint16_t len,
                                      uint32_t timeout_ms);

static IntEEPROM_Result_t my_i2c_write(uint16_t dev_addr,
                                       uint32_t mem_addr,
                                       uint8_t mem_addr_size,
                                       const uint8_t *buf,
                                       uint16_t len,
                                       uint32_t timeout_ms);

static void my_delay_ms(uint32_t ms);

// 2) 填写配置并初始化
IntEEPROM24xx_Config_t cfg = {
    .dev_addr = (0x50u << 1),
    .page_size = 8u,
    .mem_addr_size = 1u,
    .write_cycle_ms = 6u,
    .timeout_ms = 100u,
};

Int_EEPROM24xx_Init(&cfg, my_i2c_read, my_i2c_write, my_delay_ms);

// 3) 读写
Int_EEPROM24xx_Write(0x00, tx_buf, tx_len);
Int_EEPROM24xx_Read(0x00, rx_buf, rx_len);
```

## 4. 如何移植到新 MCU

只需要做 4 步：

1. 拷贝 `Int_EEPROM24xx.h/.c` 到你的工程。  
2. 用目标平台 SDK 实现 3 个函数（读、写、毫秒延时）。  
3. 按芯片手册填写配置参数：
   - `dev_addr`
   - `page_size`
   - `mem_addr_size`
   - `write_cycle_ms`
4. 调用 `Int_EEPROM24xx_Init` 一次后，即可直接调用读写函数。

## 5. 关键参数建议

- `page_size`：必须与 EEPROM 手册一致。
- `mem_addr_size`：
  - 小容量常见 1
  - 大容量常见 2
- `write_cycle_ms`：建议按 datasheet，通常 5~10ms。
- `dev_addr`：根据你的 I2C 驱动要求使用 7bit 地址或左移地址。

## 6. 常见问题排查

- 写后读不一致：优先检查 `write_cycle_ms` 是否太小。  
- 连续写入中间数据错位：优先检查 `page_size` 是否配置错误。  
- 初始化失败：检查回调函数是否传空、`mem_addr_size` 是否为 1/2。

## 7. 当前工程中的使用点

本项目在 `interface/W5500/Int_NetCfg.c` 中调用该模块，用于网络参数 EEPROM 持久化。
