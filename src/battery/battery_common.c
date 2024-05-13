
#include "battery_common.h"

#include <zephyr/kernel.h>
#include <zephyr/bluetooth/services/bas.h>

static uint8_t battery_level = 60;
struct k_timer timer;
void timmer_handler(struct k_timer *);

int battery_init(void)
{
    k_timer_init(&timer, timmer_handler, NULL);
    k_timer_start(&timer, K_SECONDS(1), K_SECONDS(1));

    return 0;
}

int battery_deinit(void)
{
    return 0;
}

uint8_t battery_get_level(void)
{
    return battery_level;
}

void battery_set_level(uint8_t level)
{
    battery_level = level;
}

void timmer_handler(struct k_timer *timer_id)
{
    battery_level--;
    if (battery_level == 0)
    {
        battery_level = 100;
    }

    printk("Battery level: %d\n", battery_level);

    #ifdef CONFIG_BT_BAS
    bt_bas_set_battery_level(battery_level);
    #endif
}