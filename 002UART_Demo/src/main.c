#include <string.h>
#include <zephyr/device.h>
#include <zephyr/drivers/clock_control/stm32_clock_control.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/led.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>

#define LED_NODE DT_COMPAT_GET_ANY_STATUS_OKAY(gpio_leds)
#define LED1_NODE DT_NODELABEL(userled)
static const struct gpio_dt_spec led_spec = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
#if !DT_NODE_EXISTS(LED1_NODE)
#error "whoops"
#endif

// #define UART_DEVICE_NODE DT_CHOSEN(zephyr_shell_uart)
#define UART_DEVICE_NODE DT_CHOSEN(zephyr_console)
static const struct device *uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);

#define RECV_BUF_SIZE 128
static uint8_t recv_buf[RECV_BUF_SIZE];
static int recv_cnt;

static volatile bool data_received = false;
static uint8_t rx_buf[64];
static size_t rx_buf_pos = 0;
// UART回调函数
void uart_isr(const struct device *dev, void *user_data) {
  while (uart_irq_update(dev) && uart_irq_is_pending(dev)) {
    if (uart_irq_rx_ready(dev)) {
      uint8_t c;
      int recv_len = uart_fifo_read(dev, &c, 1);
      if (recv_len > 0) {
        rx_buf[rx_buf_pos++] = c;
        if (c == '\n' || rx_buf_pos >= sizeof(rx_buf)) {
          data_received = true;
          uart_irq_rx_disable(dev);
        }
      }
    }
  }
}

void main(void) {
  if (!device_is_ready(uart_dev)) {
    printk("UART device not ready\n");
    return;
  } else {
    printk("UART device ready: %s\n", uart_dev->name); // 打印设备名称
  }
  // 配置UART参数：115200 8N1
  struct uart_config uart_cfg = {
      .baudrate = 115200,
      .data_bits = UART_CFG_DATA_BITS_8,
      .parity = UART_CFG_PARITY_NONE,
      .stop_bits = UART_CFG_STOP_BITS_1,
      .flow_ctrl = UART_CFG_FLOW_CTRL_NONE,
  };
  uart_configure(uart_dev, &uart_cfg);

  // 注册回调并启用接收中断
  // int ret = uart_callback_set(uart_dev, uart_cb, NULL);不支持异步接收
  int ret = uart_irq_callback_user_data_set(uart_dev, uart_isr, NULL);
  if (ret < 0) {
    printk("Failed to set UART callback: %d\n", ret);
    return;
  }
  uart_irq_rx_enable(uart_dev);
  // 打印系统时钟频率
  printk("System Clock: %d Hz\n", CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC);
  // 强制发送测试数据（绕过中断）
  const char *test_msg = "Test\n";
  for (int i = 0; i < 5; i++) {
    uart_poll_out(uart_dev, test_msg[i]);
    k_busy_wait(100); // 延时确保字符间隔
  }

  if (!device_is_ready(led_spec.port)) {
    printk("LED 设备未就绪\n");
    return 0;
  }
  gpio_pin_configure_dt(&led_spec, GPIO_OUTPUT_ACTIVE);
  while (1) {
    if (data_received) {
      rx_buf[rx_buf_pos] = '\0'; // 确保字符串以空字符结尾
      printk("Received: %s", rx_buf);
      rx_buf_pos = 0;
      data_received = false;
      uart_irq_rx_enable(uart_dev); // 重新启用接收中断
    }
    gpio_pin_toggle_dt(&led_spec);
    k_sleep(K_MSEC(100));
  }
}