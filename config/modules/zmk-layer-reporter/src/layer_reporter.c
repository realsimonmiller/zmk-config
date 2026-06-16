#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>

#include <raw_hid/events.h>

#include <zmk/event_manager.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/events/position_state_changed.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/battery.h>
#include <zmk/hid.h>
#include <zmk/keymap.h>
#include <zmk/split/central.h>
#include <zmk/usb.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#define TOTEM_HID_PROTOCOL_VERSION 0x02
#define TOTEM_HID_KIND_LAYER 0x01
#define TOTEM_HID_KIND_HEARTBEAT 0x02
#define TOTEM_HID_NUM_LAYERS 6
#define TOTEM_HID_REPORT_SIZE 32
#define TOTEM_HID_BATTERY_UNAVAILABLE 0xFF
#define TOTEM_HID_HELD_BITMAP_OFFSET 12
#define TOTEM_HID_HELD_BITMAP_BYTES 8
#define TOTEM_HID_LEFT_SOURCE 0
#define TOTEM_HID_RIGHT_SOURCE 1

/*
 * Raw HID descriptor intent, matching totem/docs/HID_PROTOCOL.md and
 * zzeneg/zmk-raw-hid defaults: Usage Page 0xFF60, Usage 0x61, 32-byte report.
 * zzeneg/zmk-raw-hid exposes transmission through raw_hid_sent_event;
 * include/raw_hid/events.h declares raw_hid_sent_event and the generated
 * raise_raw_hid_sent_event(...) API, while src/usb_hid.c subscribes to that
 * event and writes the HID IN report.
 * Source: https://github.com/zzeneg/zmk-raw-hid/blob/main/include/raw_hid/events.h
 */
static uint8_t report[TOTEM_HID_REPORT_SIZE];
static uint8_t held_positions[DIV_ROUND_UP(ZMK_KEYMAP_LEN, 8)];

static uint8_t battery_level_to_wire(uint8_t level) {
    if (level == 0) {
        return TOTEM_HID_BATTERY_UNAVAILABLE;
    }

    if (level > 100) {
        return 100;
    }

    return level;
}

static uint8_t peripheral_battery_level_to_wire(uint8_t source) {
    uint8_t level;

    if (zmk_split_central_get_peripheral_battery_level(source, &level) < 0) {
        return TOTEM_HID_BATTERY_UNAVAILABLE;
    }

    return battery_level_to_wire(level);
}

static void copy_held_positions_to_report(void) {
    for (size_t i = 0; i < TOTEM_HID_HELD_BITMAP_BYTES; i++) {
        if (i < sizeof(held_positions)) {
            report[TOTEM_HID_HELD_BITMAP_OFFSET + i] = held_positions[i];
        }
    }
}

static void build_report(uint8_t kind) {
    memset(report, 0, sizeof(report));

    uint32_t layer_state = zmk_keymap_layer_state();

    report[0] = TOTEM_HID_PROTOCOL_VERSION;
    report[1] = kind;
    report[2] = (uint8_t)(layer_state & 0xFF);
    report[3] = (uint8_t)((layer_state >> 8) & 0xFF);
    report[4] = (uint8_t)((layer_state >> 16) & 0xFF);
    report[5] = (uint8_t)((layer_state >> 24) & 0xFF);
    report[6] = (uint8_t)zmk_keymap_highest_layer_active();
    report[7] = TOTEM_HID_NUM_LAYERS;
    report[8] = (uint8_t)zmk_hid_get_explicit_mods();
    report[9] = battery_level_to_wire(zmk_battery_state_of_charge());
    report[10] = peripheral_battery_level_to_wire(TOTEM_HID_LEFT_SOURCE);
    report[11] = peripheral_battery_level_to_wire(TOTEM_HID_RIGHT_SOURCE);
    copy_held_positions_to_report();
}

static void send_report(uint8_t kind) {
    if (IS_ENABLED(CONFIG_USB_DEVICE_STACK) && !zmk_usb_is_powered()) {
        return;
    }

    build_report(kind);
    raise_raw_hid_sent_event((struct raw_hid_sent_event){
        .data = report,
        .length = sizeof(report),
    });
}

static void heartbeat_timer_handler(struct k_timer *timer) { send_report(TOTEM_HID_KIND_HEARTBEAT); }

K_TIMER_DEFINE(heartbeat_timer, heartbeat_timer_handler, NULL);

static int layer_reporter_listener(const zmk_event_t *eh) {
    if (as_zmk_layer_state_changed(eh) != NULL) {
        send_report(TOTEM_HID_KIND_LAYER);
        return ZMK_EV_EVENT_BUBBLE;
    }

    const struct zmk_position_state_changed *position_ev = as_zmk_position_state_changed(eh);
    if (position_ev != NULL) {
        if (position_ev->position < ZMK_KEYMAP_LEN) {
            WRITE_BIT(held_positions[position_ev->position / 8], position_ev->position % 8,
                      position_ev->state);
        }

        send_report(TOTEM_HID_KIND_LAYER);
        return ZMK_EV_EVENT_BUBBLE;
    }

    if (as_zmk_battery_state_changed(eh) != NULL) {
        send_report(TOTEM_HID_KIND_LAYER);
        return ZMK_EV_EVENT_BUBBLE;
    }

    if (as_zmk_peripheral_battery_state_changed(eh) != NULL) {
        send_report(TOTEM_HID_KIND_LAYER);
        return ZMK_EV_EVENT_BUBBLE;
    }

    if (as_zmk_usb_conn_state_changed(eh) != NULL) {
        send_report(TOTEM_HID_KIND_LAYER);
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(layer_reporter, layer_reporter_listener);
ZMK_SUBSCRIPTION(layer_reporter, zmk_battery_state_changed);
ZMK_SUBSCRIPTION(layer_reporter, zmk_layer_state_changed);
ZMK_SUBSCRIPTION(layer_reporter, zmk_peripheral_battery_state_changed);
ZMK_SUBSCRIPTION(layer_reporter, zmk_position_state_changed);
ZMK_SUBSCRIPTION(layer_reporter, zmk_usb_conn_state_changed);

static int layer_reporter_init(void) {
    k_timer_start(&heartbeat_timer, K_SECONDS(5), K_SECONDS(5));
    send_report(TOTEM_HID_KIND_LAYER);

    return 0;
}

SYS_INIT(layer_reporter_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
