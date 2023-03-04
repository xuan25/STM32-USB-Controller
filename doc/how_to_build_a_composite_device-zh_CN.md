# 如何创建一个 USB 复合设备

这篇文档主要阐述如何实现一个 USB HID 和 USB MIDI 的复合设备。

在接下来的段落中，首先，独立的 HID 设备和 MIDI 设备将会被分别实现并测试。之后，两个设备将会被合并为一个 USB 复合设备。

---

## 基本概念

### USB 设备描述符

USB 设备描述符被用来描述一个 USB 设备的物理特性和通信特性，其会在 USB 设备连接到主机设备的时刻被交换。其按层级被分为 设备（Device）描述，配置（Configuration）描述，界面（Interface）描述 和 终结点（Endpoint）描述。每个 USB 设备拥有一个设备描述，其描述了设备的基础物理性质，如功耗等。一个设备描述下，还可以包括多个配置描述。配置描述被用于描述设备的运行模式。在设备运行期间，会有一个配置被激活使用。但通常来说，一个 USB 设备只会包含一个配置描述。一个配置描述下，可以包含多个界面描述。界面描述用于定义设备的物理、功能特征，以及部分通信特征。一般设备会存在一个界面描述，而存在多个界面描述的设备被称为复合设备。每个界面描述下，可以包含多个终结点描述，这些终结点描述用于描述设备的通信方式。

因此，一个复合 USB 设备的描述符的结构可以为：

- Device
  - Configuration (Default)
    - Interface 1 (HID)
      - Endpoint 1 (IN)
      - Endpoint 2 (OUT)
    - Interface 2 (MIDI)
      - Endpoint 3 (IN)
      - Endpoint 4 (OUT)

### HID 报告描述符

HID 报告描述符用于进一步描述一个 HID 设备的物理特性和交互特性。该描述符的交换发生在 USB 设备描述符交换之后。

---

## 创建一个 Custom HID 设备的项目

在这一章节，CubeMX 中的项目生成配置以及基本实现将会被阐述。

### 基础项目配置

1. 配置 USB 模式
   - `USB_OTG_FS`
     - `Mode`: Device_Only
2. 配置 USB 设备类
   - `USB_DEVICE`
     - `Class For FS IP`: Custom Human Interface Device Class (HID)
3. 配置外部晶振
   - `RCC`
     - `High Speed Clock (HSE)`: Crystal/Ceramic Resonator

### 时钟配置

由于 USB 协议对时钟频率精度要求较高，因此外部晶振将会被使用。

1. `Input Frequency (HSE)`: 8 (按需调整)
2. `PLL Source Mux`: HSE
3. `HCLK`: 168
4. 其余配置按需调整

### 自定义 HID 报告

1. 在 [USB_DEVICE/App/usbd_custom_hid_if.c](../USB_DEVICE/App/usbd_custom_hid_if.c) 中按需修改 HID 报告描述符 `CUSTOM_HID_ReportDesc_FS`
2. 根据自定义 HID 报告描述符，按需修改项目生成配置中 `USB_DEVICE` 中的参数 `USBD_CUSTOM_HID_REPORT_DESC_SIZE` 和 `USBD_CUSTOMHID_OUTREPORT_BUF_SIZE` 并重新生成项目

### 测试 HID 设备功能

在 [Core/Src/main.c]() 中编写测试用例，编译上传代码并测试 HID 设备功能。

---

## 实现 MIDI 设备

在这一章节，一个基础 MIDI 设备的实现将会被阐述。（参考：[midi-box-stm32](https://github.com/Hypnotriod/midi-box-stm32)）

## 编写 MIDI 设备库

仿照 Custom HID 编写用于 MIDI 设备的类库，包括对应的 USB 设备描述符，以及处理函数。

参见：

- [Middlewares/ST/STM32_USB_Device_Library/Class/MIDI/Inc/usbd_midi.h](../Middlewares/ST/STM32_USB_Device_Library/Class/MIDI/Inc/usbd_midi.h)
- [Middlewares/ST/STM32_USB_Device_Library/Class/MIDI/Src/usbd_midi.c](../Middlewares/ST/STM32_USB_Device_Library/Class/MIDI/Src/usbd_midi.c)
- [USB_DEVICE/App/usbd_midi_if.h](../USB_DEVICE/App/usbd_midi_if.h)
- [USB_DEVICE/App/usbd_midi_if.c](../USB_DEVICE/App/usbd_midi_if.c)

### 修改 USB 设备初始化流程

修改 [USB_DEVICE/App/usb_device.c](../USB_DEVICE/App/usb_device.c)

```c
#include "usbd_midi.h"
...
// if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_HID) != USBD_OK)
if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_MIDI) != USBD_OK)
```

修改 [USB_DEVICE/Target/usbd_conf.c](../USB_DEVICE/Target/usbd_conf.c)

```c
#include "usbd_midi.h"
...
// static uint32_t mem[(sizeof(USBD_HID_HandleTypeDef)/4)+1];
static uint32_t mem[(sizeof(USBD_MIDI_HandleTypeDef)/4)+1];
```

### 测试 MIDI 设备功能

在 [Core/Src/main.c](../Core/Src/main.c) 中编写测试用例，编译上传代码并测试 MIDI 设备功能。

---

## 实现合成设备

在这一章节，一个由上述两种设备构成的合成设备的实现将会被阐述。

### 编写合成设备类中间件

参见：

- [Middlewares/ST/STM32_USB_Device_Library/Class/Composite/Inc/usbd_composite.h](../Middlewares/ST/STM32_USB_Device_Library/Class/Composite/Inc/usbd_composite.h)
- [Middlewares/ST/STM32_USB_Device_Library/Class/Composite/Src/usbd_composite.c](../Middlewares/ST/STM32_USB_Device_Library/Class/Composite/Src/usbd_composite.c)

在该中间件中，合成设备的 USB 设备描述符将会被声明。在该描述符中，注意 Interface 的数量应被声明为 2，并且两个 Interface 的 Interface ID 应被赋予为不同的值。除此之外，还需要注意两个 Interface 的 Endpoint 地址应被适当的偏移，以防止两个 Interface 上 Endpoint 的地址产生冲突。

对于 USB 处理函数，除了设备描述符相关的函数之外，先前实现 HID 和 MIDI 处理函数可以被重复利用，可以对他们进行包装和路由。在这些函数中，数据包的 Endpoint 或 Interface ID 应被判断，之后级联调用 HID 或 MIDI 中间件的处理函数实现。

### 修改 USB 设备初始化流程

修改 [USB_DEVICE/App/usb_device.c](../USB_DEVICE/App/usb_device.c)

```c
#include "usbd_composite.h"
...
void MX_USB_DEVICE_Init(void) {
  if (USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_COMPOSITE_ClassDriver) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_CUSTOM_HID_RegisterInterface(&hUsbDeviceFS, &USBD_CustomHID_fops_FS) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_Start(&hUsbDeviceFS) != USBD_OK)
  {
    Error_Handler();
  }
}
```

修改 [USB_DEVICE/Target/usbd_conf.h](../USB_DEVICE/Target/usbd_conf.h)

```c
#define USBD_MAX_NUM_INTERFACES     2U  // 不影响功能，但为了保持一致性，建议按需修改
```

修改 [USB_DEVICE/Target/usbd_conf.c](../USB_DEVICE/Target/usbd_conf.c)

```c
#include "usbd_composite.h"
...
hpcd_USB_OTG_FS.Init.dev_endpoints = 6;  // 不影响功能，但为了保持一致性，建议按需修改
...
  HAL_PCDEx_SetRxFiFo(&hpcd_USB_OTG_FS, 0x80);
  HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_FS, 0, 0x40);
  HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_FS, 1, 0x40);
  HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_FS, 2, 0x40);  // 由于复合设备使用多个 Endpoint，对应的缓冲区需要被添加。注意对各个缓冲区的尺寸进行调整，总和不要超出硬件容量限制。
...
static uint32_t mem[(MAX_CLASS_DATA_SIZE/4)+1]; // 这里为简化内存分配，一个固定缓冲区被使用，其应可以同时容纳两者的HandleTypeDef
```

### 测试复合设备功能

在 [Core/Src/main.c](../Core/Src/main.c) 中编写测试用例，编译上传代码并测试复合设备功能。

---

## END

至此，一个复合 USB 设备的基本框架已被实现。


## Reference

- [Introduction to USB with STM32](https://wiki.st.com/stm32mcu/wiki/Introduction_to_USB_with_STM32)
- [MOOC - STM32 USB training - STMicroelectronics](https://www.youtube.com/watch?v=rI3yBmnfAZU&list=PLnMKNibPkDnFFRBVD206EfnnHhQZI4Hxa)
- [midi-box-stm32](https://github.com/Hypnotriod/midi-box-stm32)
- [CDC + MSC USB Composite Device on STM32 HAL](https://sudonull.com/post/68144-CDC-MSC-USB-Composite-Device-on-STM32-HAL)
- [STM32 F4 composite usb device with 2 Bulk Interfaces, correct FIFO configuration](https://stackoverflow.com/questions/45851990/stm32-f4-composite-usb-device-with-2-bulk-interfaces-correct-fifo-configuration)
