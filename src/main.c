#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/kernel.h>
#include <soc.h>

#include "ble/ble_common.h"



int main()
{
    int err = 0;

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

    while (1)
    {
        k_sleep(K_MSEC(1000));
        printk("Ping\n");
    }

    return 0;
}