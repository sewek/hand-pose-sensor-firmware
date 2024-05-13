
#include "ble_common.h"

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <bluetooth/gatt_dm.h>

// Helper functions
static void ble_accept_pairing(void);
static void ble_reject_pairing(void);

// Callbacks
static void ble_ready_cb(int);
static void ble_connected_cb(struct bt_conn *, uint8_t);
static void ble_disconnected_cb(struct bt_conn *, uint8_t);
static void ble_security_changed_cb(struct bt_conn *, bt_security_t, uint8_t);
static void ble_auth_cancel_cb(struct bt_conn *);
static void ble_auth_confirm_cb(struct bt_conn *);
static void ble_auth_pairing_complete_cb(struct bt_conn *, bool);
static void ble_auth_pairing_failed_cb(struct bt_conn *, enum bt_security_err);
static void ble_gatt_mtu_updated(struct bt_conn *, uint16_t, uint16_t);
static void ble_gatt_discover_completed(struct bt_gatt_dm *, void *);
static void ble_gatt_discover_service_not_found(struct bt_conn *, void *);
static void ble_gatt_discover_error_found(struct bt_conn *, int, void *);

// Variables
static struct bt_conn *pairing_confirmation_conn;
static const struct bt_data advertisement_data[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, (sizeof(CONFIG_BT_DEVICE_NAME) - 1)),
};
static const struct bt_data scan_data[] = {
    BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, (sizeof(CONFIG_BT_DEVICE_NAME) - 1)),
};
static struct bt_conn_auth_cb conn_auth_callbacks = {
    .cancel = ble_auth_cancel_cb,
    .pairing_confirm = ble_auth_confirm_cb,
};
static struct bt_conn_auth_info_cb conn_auth_info_callbacks = {
    .pairing_complete = ble_auth_pairing_complete_cb,
    .pairing_failed = ble_auth_pairing_failed_cb};
static struct bt_gatt_cb gatt_callbacks = {
	.att_mtu_updated = ble_gatt_mtu_updated
};
static struct bt_gatt_dm_cb discover_callbacks = {
    .completed = ble_gatt_discover_completed,
    .service_not_found = ble_gatt_discover_service_not_found,
    .error_found = ble_gatt_discover_error_found,
};
BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected        = ble_connected_cb,
	.disconnected     = ble_disconnected_cb,
	.security_changed = ble_security_changed_cb,
};

int ble_init(void)
{
    int err = 0;

    err = bt_enable(&ble_ready_cb);
    if (err)
    {
        printk("Bluetooth init failed (err %d)\n", err);
        return err;
    }

    err = bt_conn_auth_cb_register(&conn_auth_callbacks);
    if (err)
    {
        printk("Failed to register authorization callback (err %d)\n", err);
        return err;
    }

    err = bt_conn_auth_info_cb_register(&conn_auth_info_callbacks);
    if (err)
    {
        printk("Failed to register authorization info callback (err %d)\n", err);
        return err;
    }

    return err;
}

int ble_advertise(void)
{
    int err = 0;

    err = bt_le_adv_start(BT_LE_ADV_CONN, advertisement_data, ARRAY_SIZE(advertisement_data), scan_data, ARRAY_SIZE(scan_data));
    if (err)
    {
        printk("Advertising failed to start (err %d)\n", err);
        return err;
    }
}

// Private functions

static void ble_ready_cb(int err)
{
    if (err)
    {
        return;
    }

    printk("Bluetooth enabled\n");
}

static void ble_connected_cb(struct bt_conn *conn, uint8_t err)
{
    char addr[BT_ADDR_LE_STR_LEN];
    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    if (err)
    {
        printk("Failed to connect to %s (err %d)\n", addr, err);
        return;
    }

    printk("Connected to %s\n", addr);
}

static void ble_disconnected_cb(struct bt_conn *conn, uint8_t reason)
{
    char addr[BT_ADDR_LE_STR_LEN];
	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    printk("Disconnected from %s (reason %d)\n", addr, reason);
}

static void ble_security_changed_cb(struct bt_conn *conn, bt_security_t level, uint8_t err)
{
    char addr[BT_ADDR_LE_STR_LEN];
    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    if (err)
    {
        printk("Security failed to change for %s, level %d, err %d\n", addr, level, err);
        return;
    }

    printk("Security changed for %s, level %d, err %d\n", addr, level, err);
}

static void ble_accept_pairing(void)
{
    int err = 0;

    struct bt_conn *conn = pairing_confirmation_conn;
    pairing_confirmation_conn = NULL;

    if (!conn)
    {
        printk("No connection to confirm pairing\n");
        return;
    }

    err = bt_conn_auth_pairing_confirm(conn);
    if (err)
    {
        printk("Failed to confirm the pairing: %d\n", err);
    }
    else
    {
        printk("Pairing confirmed\n");
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
        printk("No connection to confirm pairing\n");
        return;
    }

    err = bt_conn_auth_cancel(conn);
    if (err)
    {
        printk("Failed to reject the pairing: %d\n", err);
    }
    else
    {
        printk("Pairing rejected\n");
    }

    bt_conn_unref(conn);
    conn = NULL;
}

static void ble_auth_cancel_cb(struct bt_conn *conn)
{
    char addr[BT_ADDR_LE_STR_LEN];
    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    printk("Authorization cancelled for %s\n", addr);
}

static void ble_auth_confirm_cb(struct bt_conn *conn)
{
    char addr[BT_ADDR_LE_STR_LEN];
    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    __ASSERT_NO_MSG(!pairing_confirmation_conn);
    pairing_confirmation_conn = bt_conn_ref(conn);

    printk("Confirm pairing for %s\n", addr);
    printk("Auto confirm pairing...\n");
    ble_accept_pairing();
}

static void ble_auth_pairing_complete_cb(struct bt_conn *conn, bool bonded)
{
    char addr[BT_ADDR_LE_STR_LEN];
    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    printk("Pairing completed for %s, bonded: %d\n", addr, bonded);
}

static void ble_auth_pairing_failed_cb(struct bt_conn *conn, enum bt_security_err reason)
{
    char addr[BT_ADDR_LE_STR_LEN];
    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    printk("Pairing failed for %s, reason: %d\n", addr, reason);

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

    printk("MTU updated for %s, tx: %d, rx: %d\n", addr, tx, rx);
}

static void ble_gatt_discover_completed(struct bt_gatt_dm *dm, void *ctx)
{
    printk("Discovery completed\n");
}

static void ble_gatt_discover_service_not_found(struct bt_conn *conn, void *ctx)
{
    printk("Service not found\n");
}

static void ble_gatt_discover_error_found(struct bt_conn *conn, int err, void *ctx)
{
    printk("Discovery error: %d\n", err);
}