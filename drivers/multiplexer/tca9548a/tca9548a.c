#include "tca9548a.h"

#include <zephyr/sys/printk.h>

int validate_init(multiplex_init_t *init)
{
    if (init == NULL)
    {
        printk("Multiplexer init failed: init is NULL\n");
        return 1;
    }

    if (init->i2c_dev == NULL)
    {
        printk("Multiplexer init failed: i2c_dev is NULL\n");
        return 2;
    }

    if (init->address == 0)
    {
        init->address = MULTIPLEX_DEFAULT_ADDRESS;
    }

    if (!device_is_ready(init->i2c_dev))
    {
        printk("Multiplexer init failed: i2c_dev is not ready\n");
        return 3;
    }

    return 0;
}

int multiplex_init(multiplex_init_t *init)
{
    int ret = validate_init(init);
    if (ret)
    {
        return ret;
    }

    uint32_t i2c_cfg = I2C_SPEED_SET(I2C_SPEED_STANDARD) | I2C_MODE_CONTROLLER;
    if (i2c_configure(init->i2c_dev, &i2c_cfg))
    {
        printk("Multiplexer init failed: i2c_configure failed\n");
        return 4;
    }

    return 0;
}

int multiplex_deinit(multiplex_init_t *init)
{
    int ret = validate_init(init);
    if (ret)
    {
        return ret;
    }

    // TODO: Deinit i2c

    return 0;
}

int multiplex_select_channel(multiplex_init_t *init, uint8_t channel)
{
    int ret = validate_init(init);
    if (ret)
    {
        return ret;
    }

    if (channel == 0)
    {
        printk("Multiplexer select channel failed: channel is 0\n");
        return 4;
    }

    if (channel > 0x80)
    {
        printk("Multiplexer select channel failed: channel is greater than 0x80\n");
        return 5;
    }

    init->channels |= (1 << channel);

    uint8_t data[1] = {init->channels};
    struct i2c_msg msgs[1] = {
        {
            .buf = data,
            .len = 1,
            .flags = I2C_MSG_WRITE,
        },
    };

    if (i2c_transfer(init->i2c_dev, msgs, 1, init->address))
    {
        printk("Multiplexer select channel failed: i2c_transfer failed\n");
        return 6;
    }

    return 0;
}

int multiplex_deselect_channel(multiplex_init_t *init, uint8_t channel)
{
    int ret = validate_init(init);
    if (ret)
    {
        return ret;
    }

    if (channel == 0)
    {
        printk("Multiplexer select channel failed: channel is 0\n");
        return 4;
    }

    if (channel > 0x80)
    {
        printk("Multiplexer select channel failed: channel is greater than 0x80\n");
        return 5;
    }

    init->channels ^= (1 << channel);

    uint8_t data[1] = { init->channels } struct i2c_msg msgs[1] = {
        {
            .buf = data,
            .len = 1,
            .flags = I2C_MSG_WRITE,
        }
    }

    if (i2c_transfer(init->i2c_dev, msgs, 1, init->address))
    {
        printk("Multiplexer deselect channel failed: i2c_transfer failed\n");
        return 6;
    }

    return 0;
}