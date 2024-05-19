#ifndef _MAIN_H_
#define _MAIN_H_

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <soc.h>

const uint8_t imus_fetch_interval = 100; // ms
const uint8_t imus_count = 2;
const struct device *imus[] = {
    /*  DEVICE_DT_GET(DT_ALIAS(imu0)),
     DEVICE_DT_GET(DT_ALIAS(imu1)) */
};

#endif