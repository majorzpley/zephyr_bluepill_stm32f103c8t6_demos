# zephyr_bluepill-stm32f103c8t6-demos

- 此项目仅仅针对bluepill最小开发板的测试，简单介绍下开发环境：
    - 硬件资源：
        ![alt text](004I2c_SSD1306_Demo/docs/demo.jpg)
          - ssd1306 oled 128x64
          - stm32f103c8t6 bluepill开发板
          - 烧写工具：JlinkOB
    - 软件资源：
          - VSCode
          - VScode必要插件：Zephyr IDE Extension Pack(会将插件一次性安装好)，具体环境搭建可以参照插件作者在youtube提供的指导视频。
- 演示工程介绍<br>
  **001Blink_Demo**:控制led(PC13)闪烁<br>
  **002UART_Demo**:使用串口一(PA9和PA10)收发实验<br>
  **003I2c_MPU6050_Demo**:读取陀螺仪MPU6050的I2c实验<br>
  **004I2c_SSD1306_Demo**:使用SSD1306的I2C接口来进行点亮oled实验<br>
  **005SingleWire_DS18B20_Demo**:使用PA1读取单总线设备DS18B20，虽然zephyr已经有此驱动，但是貌似是arduino架构的，没有成功，故自己添加了驱动(没有注册)<br>