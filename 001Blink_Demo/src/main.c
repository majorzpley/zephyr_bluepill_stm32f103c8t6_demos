#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/led.h>
#include <zephyr/kernel.h>

#define LED1_NODE DT_NODELABEL(userled)
static const struct gpio_dt_spec led_spec = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
#if !DT_NODE_EXISTS(LED1_NODE)
#error "whoops"
#endif

int main(void) {
  if (!device_is_ready(led_spec.port)) {
    printk("LED 设备未就绪\n");
    return 0;
  }
  gpio_pin_configure_dt(&led_spec, GPIO_OUTPUT_ACTIVE);
  while (1) {
    gpio_pin_toggle_dt(&led_spec);
    k_sleep(K_MSEC(200));
  }
}