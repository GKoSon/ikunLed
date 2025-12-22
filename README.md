# ikunLED

## 1、介绍

ikunLED 是一个基于 RT-Thread 设备驱动框架实现的 LED 设备驱动模块。该模块将 LED 封装为 `ikled_device` 设备并注册到 RT-Thread 设备管理器中，方便用户通过标准设备接口或简化 API 对 LED 进行控制。通过设备管理器获取设备句柄后，可进行 LED 状态设置、读取、翻转及闪烁等操作（参考示例函数 `ledDev_sample`）。
同时，模块提供了二次封装的简洁 API，支持通过用户自定义设备名称直接操作 LED 的开关和闪烁功能。

### 1.1、特性

- 符合 RT-Thread 设备驱动框架规范，支持设备注册与管理
- 提供标准设备接口（open/close/write/control）
- 支持 LED 状态设置（亮 / 灭）、状态读取、状态翻转操作
- 支持自定义次数和周期的闪烁功能
- 提供简化 API（`ikled_on`/`ikled_off`/`ikled_blink`），方便快速开发
- 可通过用户自定义名称区分多个 LED 设备

## 2、使用方法

### 2.1、初始化设备

通过 `ikled_init` 函数初始化 LED 设备，指定设备名称、引脚和有效电平：

```c
ikled_init("led1", rt_pin_get("PB.12"), 0);  // 初始化名为"led1"的LED设备
```

### 2.2、控制 LED

- 打开（点亮）LED：
```c
ikled_on("led1");  // 点亮名为"led1"的LED
```

- 关闭（熄灭）LED：
```c
ikled_off("led1");  // 熄灭名为"led1"的LED
```

- 闪烁控制：[闪烁设为0次=永不停止的闪烁]
```c
ikled_blink("led1", 3, 200);  // 闪烁3次，周期200ms
```

### 2.3、标准设备接口操作

也可通过 RT-Thread 标准设备接口进行操作：

```c
rt_device_t led_dev = rt_device_find("led1");  // 获取设备句柄
rt_device_open(led_dev, RT_DEVICE_OFLAG_RDWR);  // 打开设备（点亮LED）
rt_device_control(led_dev, LED_CMD_TOGGLE, NULL);  // 翻转LED状态
```

## 3、示例程序

模块内置 `ledDev_sample` 函数演示完整使用流程，可通过 RT-Thread 控制台执行：

shell

```shell
msh > ledDev_sample
```

示例流程包括：初始化设备、打开 / 关闭 LED、读取状态、翻转状态及闪烁控制。

## 4、联系方式 & 感谢

- 维护：龚金华
- 主页：[https://gitee.com/GKoSon]
- 邮箱：[782730309@qq.com](mailto:782730309@qq.com)

欢迎提交 issue 或 PR 参与模块改进与优化。