#include "imu.h"

#include <stddef.h>
#include <zephyr/types.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

uint8_t imu_init(const struct device *dev)
{
    if (dev == NULL)
    {
        printk("IMU init failed: dev is NULL\n");
        return 1;
    }

    return 0;
}
uint8_t imu_deinit(const struct device *dev)
{
    if (dev == NULL)
    {
        printk("IMU deinit failed: dev is NULL\n");
        return 1;
    }

    return 0;
}

uint8_t imu_fetch_data(const struct device *dev, imu_sample_t *sample)
{
    struct sensor_value accel[3];
    struct sensor_value gyro[3];

    if (!device_is_ready(dev))
    {
        printk("IMU process failed: dev is not ready\n");
        return 1;
    }

    if (sensor_sample_fetch(dev))
    {
        printk("IMU process failed: sensor_sample_fetch failed\n");
        return 2;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel))
    {
        printk("IMU process failed: sensor_channel_get failed\n");
        return 3;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_GYRO_XYZ, gyro))
    {
        printk("IMU process failed: sensor_channel_get failed\n");
        return 4;
    }

    memcpy(sample->name, dev->name, sizeof(dev->name));

    sample->accel[0] = sensor_value_to_double(&accel[0]);
    sample->accel[1] = sensor_value_to_double(&accel[1]);
    sample->accel[2] = sensor_value_to_double(&accel[2]);

    sample->gyro[0] = sensor_value_to_double(&gyro[0]);
    sample->gyro[1] = sensor_value_to_double(&gyro[1]);
    sample->gyro[2] = sensor_value_to_double(&gyro[2]);

    return 0;
}