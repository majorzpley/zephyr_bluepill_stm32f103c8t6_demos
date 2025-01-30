//? 此项目为MPU6050读取加速度与角速度测试所用
// brief 基于zephyr4.0
// bug 注意增加主线程的栈大小，否则printk会造成栈溢出
//- 测试通过
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define MPU6050_NODE DT_NODELABEL(mpu6050)
static const struct device *mpu6050_dev = DEVICE_DT_GET(MPU6050_NODE);

void main(void) {
  if (!device_is_ready(mpu6050_dev)) {
    printk("MPU6050 device not ready\n");
    return;
  } else {
    printk("MPU6050 device ready: %s\n", mpu6050_dev->name);
  }

  struct sensor_value accel[3];
  struct sensor_value gyro[3];
  struct sensor_value temperature;

  while (1) {
    int rc = sensor_sample_fetch(mpu6050_dev);
    if (rc != 0) {
      printk("Failed to fetch sensor data\n");
      return;
    }
    sensor_channel_get(mpu6050_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    sensor_channel_get(mpu6050_dev, SENSOR_CHAN_GYRO_XYZ, gyro);
    sensor_channel_get(mpu6050_dev, SENSOR_CHAN_DIE_TEMP, &temperature);
    if (rc == 0) {
      printk("  T %g Cel\n", sensor_value_to_double(&temperature));

      printk("  accel %f %f %f m/s/s\n", sensor_value_to_double(&accel[0]),
             sensor_value_to_double(&accel[1]),
             sensor_value_to_double(&accel[2]));
      printk("  gyro  %f %f %f rad/s\n", sensor_value_to_double(&gyro[0]),
             sensor_value_to_double(&gyro[1]),
             sensor_value_to_double(&gyro[2]));
    } else {
      printk("sample fetch/get failed: %d\n", rc);
    }
    k_sleep(K_MSEC(1000));
  }
}