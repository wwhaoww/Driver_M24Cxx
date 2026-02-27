# Driver_M24Cxx（可移植 EEPROM 驱动）

这是一个面向 M24Cxx / AT24Cxx 系列 EEPROM 的可移植驱动模块。

核心特性：

- 不依赖特定 MCU/SDK（不绑定 STM32 HAL）
- 支持页写自动拆分
- 通过回调适配底层总线

## 目录结构

- `module/Int_EEPROM24xx.h`：驱动接口定义
- `module/Int_EEPROM24xx.c`：驱动实现
- `examples/adapter_template.c`：适配层模板

## 你需要实现的 3 个函数

移植时，你只需要实现这三个底层函数，然后在配置结构体里挂接：

1. `bus_read`：从 EEPROM 读数据
2. `bus_write`：向 EEPROM 写数据
3. `delay_ms`：毫秒延时

> 这也是本仓库提交信息里的重点：**需要自己实现读、写、延时这三个函数**。

## 快速接入

### 1) 将模块加入工程

把以下文件加入你的工程：

- `module/Int_EEPROM24xx.h`
- `module/Int_EEPROM24xx.c`

### 2) 实现底层适配函数

参考 `examples/adapter_template.c`，按你的平台替换 I2C 读写与延时实现。

### 3) 初始化设备描述并调用 API

```c
static const IntEEPROM24xx_t g_eeprom = {
    .ctx = your_bus_context,
    .dev_addr = (0x50u << 1),
    .page_size = 8u,
    .mem_addr_size = 1u,
    .write_cycle_ms = 6u,
    .timeout_ms = 100u,
    .bus_read = your_bus_read,
    .bus_write = your_bus_write,
    .delay_ms = your_delay_ms,
};

Int_EEPROM24xx_Read(&g_eeprom, 0x00, rx_buf, sizeof(rx_buf));
Int_EEPROM24xx_Write(&g_eeprom, 0x00, tx_buf, sizeof(tx_buf));
```

## 参数说明

- `dev_addr`：设备地址（7bit 或左移后地址，取决于你的底层驱动规范）
- `page_size`：页大小（必须与芯片手册一致）
- `mem_addr_size`：地址字节数（1 或 2）
- `write_cycle_ms`：每次页写后的写周期等待

## 常见问题

- 写入失败：优先检查 `page_size` 和 `write_cycle_ms`
- 读写偏移错乱：检查 `mem_addr_size`（1 字节 / 2 字节）
- 跨平台报错：说明底层回调未按目标 SDK 正确实现
