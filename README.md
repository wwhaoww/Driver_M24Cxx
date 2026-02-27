# Driver_M24Cxx（可移植 EEPROM 驱动）

这是一个面向 M24Cxx / AT24Cxx 系列 EEPROM 的可移植驱动模块。

核心特性：

- 不依赖特定 MCU/SDK（不绑定 STM32 HAL）
- 支持页写自动拆分
- 用户传入 I2C 读、I2C 写、毫秒延时三个函数后即可直接使用

## 目录结构

- `module/Int_EEPROM24xx.h`：驱动接口
- `module/Int_EEPROM24xx.c`：驱动实现
- `examples/adapter_template.c`：移植模板

## 使用步骤

1. 将 `module/Int_EEPROM24xx.h/.c` 加入你的工程。
2. 在你的平台实现三个函数：
   - `your_i2c_read(...)`
   - `your_i2c_write(...)`
   - `your_delay_ms(...)`
3. 调用 `Int_EEPROM24xx_Init(...)` 完成初始化。
4. 直接调用 `Int_EEPROM24xx_Read/Write` 进行访问。

## 示例

```c
IntEEPROM24xx_Config_t cfg = {
    .dev_addr = (0x50u << 1),
    .page_size = 8u,
    .mem_addr_size = 1u,
    .write_cycle_ms = 6u,
    .timeout_ms = 100u,
};

Int_EEPROM24xx_Init(&cfg, your_i2c_read, your_i2c_write, your_delay_ms);

Int_EEPROM24xx_Write(0x00, tx_buf, tx_len);
Int_EEPROM24xx_Read(0x00, rx_buf, rx_len);
```

## 参数说明

- `dev_addr`：设备地址（7bit 或左移后地址，取决于底层驱动规范）
- `page_size`：页大小（必须与芯片手册一致）
- `mem_addr_size`：地址字节数（1 或 2）
- `write_cycle_ms`：每次页写后的写周期等待
- `timeout_ms`：底层总线访问超时

## 注意事项

- 驱动内部会自动按页拆分写入，避免跨页写异常。
- 若你的器件使用 2 字节地址，请设置 `mem_addr_size = 2`。
- 若写后立即读失败，请适当增大 `write_cycle_ms`。
