
#ifndef _TCA9548A_H_
#define _TCA9548A_H_

#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>

#define MULTIPLEX_DEFAULT_ADDRESS 0x70

#define MULTIPLEX_CHANNEL_0 0x01
#define MULTIPLEX_CHANNEL_1 0x02
#define MULTIPLEX_CHANNEL_2 0x04
#define MULTIPLEX_CHANNEL_3 0x08
#define MULTIPLEX_CHANNEL_4 0x10
#define MULTIPLEX_CHANNEL_5 0x20
#define MULTIPLEX_CHANNEL_6 0x40
#define MULTIPLEX_CHANNEL_7 0x80

typedef struct
{
    const struct device *i2c_dev;
    uint8_t address;
    uint8_t channels;
} multiplex_init_t;

int multiplex_init(multiplex_init_t *);
int multiplex_deinit(multiplex_init_t *);
int multiplex_select_channel(multiplex_init_t *, uint8_t);
int multiplex_deselect_channel(multiplex_init_t *, uint8_t);

#endif