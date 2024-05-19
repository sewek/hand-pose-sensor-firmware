
#ifndef _IMU_H_
#define _IMU_H_

#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#define IMU_SAMPLES_COUNT 2

typedef struct
{
    char name[16];
    double accel[3];
    double gyro[3];
} imu_sample_t;

extern imu_sample_t imu_samples[IMU_SAMPLES_COUNT];

uint8_t
imu_init(const struct device *dev);
uint8_t imu_deinit(const struct device *dev);
uint8_t imu_fetch_data(const struct device *dev, imu_sample_t *sample);

#endif