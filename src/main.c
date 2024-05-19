#include "main.h"

#include "ble/ble_common.h"
#include "imu/imu.h"

int main()
{
    int err = 0;

    // Bluetooth init
    err = ble_init();
    if (err)
    {
        printk("Error initializing BLE\n");
        return err;
    }

    err = ble_start_advertising();
    if (err)
    {
        printk("Error starting advertising\n");
        return err;
    }

    printk("Bluetooth initialized\n");

    // IMUs init
    uint32_t imus_last_fetch = 0;
    imu_sample_t imu_samples[IMU_SAMPLES_COUNT] = {0};
    sensor_service_data_t sensor_service_data = {0};
    for (uint8_t i = 0; i < imus_count; i++)
    {
        err = imu_init(imus[i]);
        if (err)
        {
            printk("Error initializing IMU %d\n", i);
            return err;
        }

        printk("IMU %d initialized\n", i);
    }

    while (1)
    {
        if (k_uptime_delta(&imus_last_fetch) >= imus_fetch_interval)
        {
            for (uint8_t i = 0; i < imus_count; i++)
            {
                imu_sample_t sample;
                err = imu_fetch_data(imus[i], &sample);
                if (err)
                {
                    printk("Error fetching data from IMU %d\n", i);
                    return err;
                }

                imu_samples[i] = sample;
                printk("IMU %d: Accel: %f, %f, %f; Gyro: %f, %f, %f\n", i, sample.accel[0], sample.accel[1], sample.accel[2], sample.gyro[0], sample.gyro[1], sample.gyro[2]);
            }
            sensor_service_data.delta_time = k_uptime_delta(&imus_last_fetch);
            sensor_service_data.count = imus_count;
            memcpy(sensor_service_data.samples, imu_samples, sizeof(imu_sample_t) * IMU_SAMPLES_COUNT);

            imus_last_fetch = k_uptime_get_32();
            sensor_service_notify_data(&sensor_service_data);
        }

        k_sleep(K_MSEC(10));
    }

    return 0;
}