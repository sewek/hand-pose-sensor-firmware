
#ifndef _BATTERY_COMMON_H_
#define _BATTERY_COMMON_H_

#include <stdint.h>

int battery_init(void);
int battery_deinit(void);

uint8_t battery_get_level(void);
void battery_set_level(uint8_t level);

#endif // _BATTERY_COMMON_H_