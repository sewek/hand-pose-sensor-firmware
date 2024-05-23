//#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
/* #include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <soc.h> */
//#include <zephyr/logging/log.h>

//#include "ble/ble_common.h"
//#include "imu/imu.h"

//#define LED0_NODE DT_ALIAS(led0)

//static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

/* const uint8_t imus_fetch_interval = 100; // ms
const uint8_t imus_count = 2;
const struct device *imus[] = {0
    //DEVICE_DT_GET(DT_ALIAS(imu0)),
    //DEVICE_DT_GET(DT_ALIAS(imu1))
    }; */

/* void k_sys_fatal_error_handler(unsigned int reason, const z_arch_esf_t *esf)
{
    printf("Fatal error: %d\n", reason);
    LOG_PANIC();
    k_fatal_halt(reason);
} */
//struct bt_conn *bt_connection;
extern int ble_init(void);
extern int ble_advertise(void);


int main(void)
{
    int err = 0;

    printf("Starting application\n");
    printf("Starting application\n");
    printf("Starting application\n");
    printf("Starting application\n");
    printk("Starting application\n");
    //LOG_INF("Starting application\n");

    /* err = gpio_is_ready_dt(&led);
    if (err) {
        LOG_ERR("LED pin is not ready\n");
        printk("LED pin is not ready\n");
		return 0;
	}

	err = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if (err) {
        LOG_ERR("Error configuring LED pin\n");
        printk("Error configuring LED pin\n");
        return err;
    }

    gpio_pin_set_dt(&led, 1); */

    // Bluetooth init
    err = ble_init();
    if (err)
    {
        printf("Error initializing BLE\n");
    }

    err = ble_advertise();
    if (err)
    {
        printf("Error starting advertising\n");
    }

    //LOG_INF("Bluetooth initialized\n");
    printf("Bluetooth initialized\n");

    // IMUs init
    /* uint32_t imus_last_fetch = 0;
    imu_sample_t imu_samples[IMU_SAMPLES_COUNT] = {0};
    sensor_service_data_t sensor_service_data = {0};
    for (uint8_t i = 0; i < imus_count; i++)
    {
        err = imu_init(imus[i]);
        if (err)
        {
            LOG_ERR("Error initializing IMU %d\n", i);
            printk("Error initializing IMU %d\n", i);
            return err;
        }

        LOG_INF("IMU %d initialized\n", i);
        printk("IMU %d initialized\n", i);
    } */

    //gpio_pin_set_dt(&led, 0);

    printf("Starting loop\n");
    while (1)
    {
        /* if (k_uptime_delta(&imus_last_fetch) >= imus_fetch_interval)
        {
            for (uint8_t i = 0; i < imus_count; i++)
            {
                imu_sample_t sample;
                err = imu_fetch_data(imus[i], &sample);
                if (err)
                {
                    LOG_ERR("Error fetching data from IMU %d\n", i);
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
        } */

        k_sleep(K_MSEC(1000));
        //LOG_INF("Sleeping\n");
        printf("Sleeping\n");
    }

    return 0;
}
