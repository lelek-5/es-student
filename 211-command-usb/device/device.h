#pragma once

#include <stdint.h>
#include "pico/types.h"

#define DEVICE_NAME "es-led-module"
#define FIRMWARE_VERSION "1.0.0"
#define DEVICE_PROJECT "211-command-usb"
#define DEVICE_REPO "https://github.com/lelek-5/es-student"

#ifndef DEVICE_BOARD
#define DEVICE_BOARD "unknown"
#endif

struct info_t {
    uint32_t serial;
    uint8_t version;
    char name[16];
};

extern const struct info_t device_card;

void device_info(void);
void dev_info(void);