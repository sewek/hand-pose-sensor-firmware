#include "sensor_service.h"
#include "../ble_common.h"
#include "../../imu/imu.h"

sensor_service_data_t sensor_service_data = {0};
struct bt_conn *bt_connection;
volatile bool notify_enable = true;

uint8_t sensor_service_notify_data(sensor_service_data_t *data)
{
    if (bt_connection == NULL)
    {
        printk("No connection\n");
        return 0;
    }

    if (!notify_enable)
    {
        printk("Notifications are disabled\n");
        return 0;
    }

    memcpy(&sensor_service_data, data, sizeof(sensor_service_data_t));

    if (bt_gatt_notify(NULL, sensor_svc.attrs[4], data, sizeof(*data)))
    {
        printk("Failed to notify\n");
        return 1;
    }

    return 0;
}

static ssize_t sensor_char_data_read(struct bt_conn *conn,
                                     const struct bt_gatt_attr *attr,
                                     void *buf,
                                     uint16_t len,
                                     uint16_t offset)
{
    const char *value = attr->user_data;

    LOG_DBG("Attribute read, handle: %u, conn: %p", attr->handle,
            (void *)conn);

    return bt_gatt_attr_read(conn, attr, buf, len, offset, value, sizeof(*value));
}

static void sensor_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    ARG_UNUSED(attr);
    notify_enable = (value == BT_GATT_CCC_NOTIFY);
    printk("Notification %s", notify_enable ? "enabled" : "disabled");
}

BT_GATT_SERVICE_DEFINE(
    sensor_svc,
    BT_GATT_PRIMARY_SERVICE(SENSOR_SERVICE_UUID_LBS),
    BT_GATT_CHARACTERISTIC(
        SENSOR_SERVICE_DATA_CHAR_UUID,
        BT_GATT_CHRC_NOTIFY | BT_GATT_CHRC_READ,
        BT_GATT_PERM_READ,
        sensor_char_data_read,
        NULL,
        &sensor_service_data),
    BT_GATT_CHARACTERISTIC(
        SENSOR_SERVICE_CONFIG_CHAR_UUID,
        BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE,
        BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
        NULL,
        NULL,
        NULL),
    BT_GATT_CCC(sensor_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE));
