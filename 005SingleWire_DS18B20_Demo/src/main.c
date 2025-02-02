#include "ds18b20.h"
#include <float.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/logging/log_ctrl.h>
#include <zephyr/timing/timing.h>

LOG_MODULE_REGISTER();

int main() {
  float temperatura;
  log_panic();
  while (1) {
    k_msleep(5);
    temperatura = ds18b20_get_temperature();

    if (temperatura == FLT_MIN) {
      LOG_ERR("Erro ao ler a temperatura do sensor");
    }
  }
}