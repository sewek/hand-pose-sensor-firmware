
#include <zephyr/sys/printk.h>
#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>


struct bt_conn *bt_connection;
static struct bt_conn *pairing_confirmation_conn;

static void ble_ready_cb(int err)
{
    if (err)
    {
        return;
    }

    printf("Bluetooth enabled\n");
}

static void ble_connected_cb(struct bt_conn *conn, uint8_t err)
{
    char addr[BT_ADDR_LE_STR_LEN];
    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    if (err)
    {
        printf("Failed to connect to %s (err %d)\n", addr, err);
        return;
    }

    bt_connection = bt_conn_ref(conn);

    printf("Connected to %s\n", addr);
}

static void ble_disconnected_cb(struct bt_conn *conn, uint8_t reason)
{
    char addr[BT_ADDR_LE_STR_LEN];
    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    if (bt_connection)
    {
        bt_conn_unref(bt_connection);
        bt_connection = NULL;
    }

    printf("Disconnected from %s (reason %d)\n", addr, reason);
}

static void ble_security_changed_cb(struct bt_conn *conn, bt_security_t level, uint8_t err)
{
    char addr[BT_ADDR_LE_STR_LEN];
    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    if (err)
    {
        printf("Security failed to change for %s, level %d, err %d\n", addr, level, err);
        return;
    }

    printf("Security changed for %s, level %d, err %d\n", addr, level, err);
}

static void ble_accept_pairing(void)
{
    int err = 0;

    struct bt_conn *conn = pairing_confirmation_conn;
    pairing_confirmation_conn = NULL;

    if (!conn)
    {
        printf("No connection to confirm pairing\n");
        return;
    }

    err = bt_conn_auth_pairing_confirm(conn);
    if (err)
    {
        printf("Failed to confirm the pairing: %d\n", err);
    }
    else
    {
        printf("Pairing confirmed\n");
    }

    bt_conn_unref(conn);
    conn = NULL;
}

static void ble_reject_pairing(void)
{
    int err = 0;

    struct bt_conn *conn = pairing_confirmation_conn;
    pairing_confirmation_conn = NULL;

    if (!conn)
    {
        printf("No connection to confirm pairing\n");
        return;
    }

    err = bt_conn_auth_cancel(conn);
    if (err)
    {
        printf("Failed to reject the pairing: %d\n", err);
    }
    else
    {
        printf("Pairing rejected\n");
    }

    bt_conn_unref(conn);
    conn = NULL;
}

static void ble_auth_cancel_cb(struct bt_conn *conn)
{
    char addr[BT_ADDR_LE_STR_LEN];
    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    printf("Authorization cancelled for %s\n", addr);
}

static void ble_auth_confirm_cb(struct bt_conn *conn)
{
    char addr[BT_ADDR_LE_STR_LEN];
    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    __ASSERT_NO_MSG(!pairing_confirmation_conn);
    pairing_confirmation_conn = bt_conn_ref(conn);

    printf("Confirm pairing for %s\n", addr);
    printf("Auto confirm pairing...\n");
    ble_accept_pairing();
}

static void ble_auth_pairing_complete_cb(struct bt_conn *conn, bool bonded)
{
    char addr[BT_ADDR_LE_STR_LEN];
    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    printf("Pairing completed for %s, bonded: %d\n", addr, bonded);
}

static void ble_auth_pairing_failed_cb(struct bt_conn *conn, enum bt_security_err reason)
{
    char addr[BT_ADDR_LE_STR_LEN];
    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    printf("Pairing failed for %s, reason: %d\n", addr, reason);

    if (pairing_confirmation_conn)
    {
        bt_conn_unref(pairing_confirmation_conn);
        pairing_confirmation_conn = NULL;
    }
}

static void ble_gatt_mtu_updated(struct bt_conn *conn, uint16_t tx, uint16_t rx)
{
    char addr[BT_ADDR_LE_STR_LEN];
    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    printf("MTU updated for %s, tx: %d, rx: %d\n", addr, tx, rx);
}

static void ble_gatt_discover_completed(struct bt_gatt_dm *dm, void *ctx)
{
    printf("Discovery completed\n");
}

static void ble_gatt_discover_service_not_found(struct bt_conn *conn, void *ctx)
{
    printf("Service not found\n");
}

static void ble_gatt_discover_error_found(struct bt_conn *conn, int err, void *ctx)
{
    printf("Discovery error: %d\n", err);
}

static bool ble_param_req(struct bt_conn *conn, struct bt_le_conn_param *param)
{
    printk("%s: int (0x%04x, 0x%04x) lat %u to %u\n", __func__,
           param->interval_min, param->interval_max, param->latency,
           param->timeout);

    return true;
}

static void ble_param_updated(struct bt_conn *conn, uint16_t interval,
                              uint16_t latency, uint16_t timeout)
{
    printk("%s: int 0x%04x lat %u to %u\n", __func__, interval,
           latency, timeout);
}


static const struct bt_data advertisement_data[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    // BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, (sizeof(CONFIG_BT_DEVICE_NAME) - 1)),
};
/* static const struct bt_data scan_data[] = {
    BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, (sizeof(CONFIG_BT_DEVICE_NAME) - 1)),
}; */
/* static struct bt_conn_auth_cb conn_auth_callbacks = {
    .cancel = ble_auth_cancel_cb,
    .pairing_confirm = ble_auth_confirm_cb,
}; */
static struct bt_conn_auth_info_cb conn_auth_info_callbacks = {
    .pairing_complete = ble_auth_pairing_complete_cb,
    .pairing_failed = ble_auth_pairing_failed_cb};
static struct bt_gatt_cb gatt_callbacks = {
    .att_mtu_updated = ble_gatt_mtu_updated};
/* static struct bt_gatt_dm_cb discover_callbacks = {
    .completed = ble_gatt_discover_completed,
    .service_not_found = ble_gatt_discover_service_not_found,
    .error_found = ble_gatt_discover_error_found,
}; */

int ble_init(void)
{
    int err = 0;

    err = bt_enable(NULL);
    if (err)
    {
        printf("Bluetooth init failed (err %d)\n", err);
        return err;
    }

    bt_gatt_cb_register(&gatt_callbacks);

    /* err = bt_conn_auth_cb_register(&conn_auth_callbacks);
    if (err)
    {
        printf("Failed to register authorization callback (err %d)\n", err);
        return err;
    }

    err = bt_conn_auth_info_cb_register(&conn_auth_info_callbacks);
    if (err)
    {
        printf("Failed to register authorization info callback (err %d)\n", err);
        return err;
    } */

    return err;
}

int ble_advertise(void)
{
    int err = 0;

    err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, advertisement_data, ARRAY_SIZE(advertisement_data), NULL, 0); // scan_data, ARRAY_SIZE(scan_data));
    if (err)
    {
        printf("Advertising failed to start (err %d)\n", err);
        return err;
    }

    return err;
}


BT_CONN_CB_DEFINE(conn_callbacks) = {
    .connected = ble_connected_cb,
    .disconnected = ble_disconnected_cb,
    .security_changed = ble_security_changed_cb,
    .le_param_req = ble_param_req,
	.le_param_updated = ble_param_updated,
};
