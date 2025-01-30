//? 此项目为SSD1306测试OLED显示所用
// brief 基于zephyr4.0
// bug 注意增加主线程的栈大小，否则printk会造成栈溢出
//- 测试通过
#include "ascii_font.c"
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/led.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define LED1_NODE DT_NODELABEL(userled)
static const struct gpio_dt_spec led_spec = GPIO_DT_SPEC_GET(LED1_NODE, gpios);

#define SSD1306_NODE DT_NODELABEL(ssd1306)
static const struct device *display = DEVICE_DT_GET(SSD1306_NODE);

static void display_print_string_to_buffer(int x, int y, uint8_t *buf,
                                           int len) {
  int start_pos = 0;
  if ((x < 0) || (x > 127) || (y < 0) || (y > 63))
    return;
  memset(buf, 0x00, len);
  // todo 打印Hello, Zephyr 4.0从32行50列开始测试
  start_pos = y / 8 * 128 + x;
  if (start_pos >= len) {
    return;
  }
  //- H
  memcpy(&buf[start_pos], &ascii_font[0][0], 16);
  start_pos += 128;
  memcpy(&buf[start_pos], &ascii_font[0][16], 16);
  start_pos += 16;
  //- e
  start_pos -= 128; // 恢复到上一行
  memcpy(&buf[start_pos], &ascii_font[1][0], 16);
  start_pos += 128;
  memcpy(&buf[start_pos], &ascii_font[1][16], 16);
  start_pos += 16;
  //- ll
  start_pos -= 128; // 恢复到上一行
  memcpy(&buf[start_pos], &ascii_font[2][0], 16);
  start_pos += 128;
  memcpy(&buf[start_pos], &ascii_font[2][16], 16);
  start_pos += 16;
  start_pos -= 128; // 恢复到上一行
  memcpy(&buf[start_pos], &ascii_font[2][0], 16);
  start_pos += 128;
  memcpy(&buf[start_pos], &ascii_font[2][16], 16);
  start_pos += 16;
  //- o
  start_pos -= 128; // 恢复到上一行
  memcpy(&buf[start_pos], &ascii_font[3][0], 16);
  start_pos += 128;
  memcpy(&buf[start_pos], &ascii_font[3][16], 16);
  start_pos += 16;
  //- Z
  start_pos -= 16 * 5;
  start_pos += 128;
  memcpy(&buf[start_pos], &ascii_font[5][0], 16);
  start_pos += 128;
  memcpy(&buf[start_pos], &ascii_font[5][16], 16);
  start_pos += 16;
  //- e
  start_pos -= 128; // 恢复到上一行
  memcpy(&buf[start_pos], &ascii_font[6][0], 16);
  start_pos += 128;
  memcpy(&buf[start_pos], &ascii_font[6][16], 16);
  start_pos += 16;
  //- p
  start_pos -= 128; // 恢复到上一行
  memcpy(&buf[start_pos], &ascii_font[7][0], 16);
  start_pos += 128;
  memcpy(&buf[start_pos], &ascii_font[7][16], 16);
  start_pos += 16;
  //- h
  start_pos -= 128; // 恢复到上一行
  memcpy(&buf[start_pos], &ascii_font[8][0], 16);
  start_pos += 128;
  memcpy(&buf[start_pos], &ascii_font[8][16], 16);
  start_pos += 16;
  //- y
  start_pos -= 128; // 恢复到上一行
  memcpy(&buf[start_pos], &ascii_font[9][0], 16);
  start_pos += 128;
  memcpy(&buf[start_pos], &ascii_font[9][16], 16);
  start_pos += 16;
  //- r
  start_pos -= 128; // 恢复到上一行
  memcpy(&buf[start_pos], &ascii_font[10][0], 16);
  start_pos += 128;
  memcpy(&buf[start_pos], &ascii_font[10][16], 16);
  start_pos += 16;
}
void main(void) {
  if (!device_is_ready(display)) {
    printk("SSD1306 device not ready\n");
    return;
  } else {
    printk("SSD1306 device ready: %s\n", display->name);
  }
  uint8_t buffer[128 * 64 / 8] = {0}; // 128x64 单色位图
  struct display_buffer_descriptor desc = {
      .width = 128, .height = 64, .pitch = 128, .buf_size = sizeof(buffer)};
  display_print_string_to_buffer(20, 20, buffer, sizeof(buffer));
  display_blanking_off(display);
  display_write(display, 0, 0, &desc, buffer);
  // todo LED注册节点初始化
  if (!device_is_ready(led_spec.port)) {
    printk("LED 设备未就绪\n");
    return 0;
  } else {
    printk("led device ready: %s\n", led_spec.port->name);
  }
  gpio_pin_configure_dt(&led_spec, GPIO_OUTPUT_ACTIVE);
  while (1) {
    gpio_pin_toggle_dt(&led_spec);
    k_sleep(K_MSEC(200));
  }
}
