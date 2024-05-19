
#ifndef _BLE_COMMON_H_
#define _BLE_COMMON_H_

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <bluetooth/gatt_dm.h>

#include "gatt_services.h"

extern struct bt_conn *bt_connection;

int ble_init(void);
int ble_advertise(void);

#endif // _BLE_COMMON_H_