#include "device.h"
#include <stdio.h>
#include <stddef.h>
#include "pico/unique_id.h"
#include "pico/version.h"
#include "hardware/regs/addressmap.h"
#include "hardware/regs/sysinfo.h"

struct info_t device_card = {
    .serial = 123456,
    .version = 1,
    .name = "device"
};

void device_info(void) {
    char board_id[PICO_UNIQUE_BOARD_ID_SIZE_BYTES * 2 + 1];
    pico_get_unique_board_id_string(board_id, sizeof(board_id));

    volatile uint32_t *chip_id = (uint32_t *)(SYSINFO_BASE + SYSINFO_CHIP_ID_OFFSET);
    uint32_t id = *chip_id;

    uint32_t manufacturer = (id & SYSINFO_CHIP_ID_MANUFACTURER_BITS) >> SYSINFO_CHIP_ID_MANUFACTURER_LSB;
    uint32_t part = (id & SYSINFO_CHIP_ID_PART_BITS) >> SYSINFO_CHIP_ID_PART_LSB;
    uint32_t revision = (id & SYSINFO_CHIP_ID_REVISION_BITS) >> SYSINFO_CHIP_ID_REVISION_LSB;

    printf("project: %s\n", DEVICE_PROJECT);
    printf("repo: %s\n", DEVICE_REPO);
    printf("board: %s\n", DEVICE_BOARD);
    printf("serial: %s\n", board_id);
    printf("chip: manufacturer 0x%03x, part 0x%04x, revision %u\n", manufacturer, part, revision);
    printf("pico-sdk: %s\n", PICO_SDK_VERSION_STRING);
}

void dev_info(void) {
    size_t size_serial = sizeof(device_card.serial);
    size_t size_version = sizeof(device_card.version);
    size_t size_name = sizeof(device_card.name);

    size_t off_serial = offsetof(struct info_t, serial);
    size_t off_version = offsetof(struct info_t, version);
    size_t off_name = offsetof(struct info_t, name);

    size_t total_fields = size_serial + size_version + size_name;
    size_t total_struct = sizeof(device_card);
    size_t padding = total_struct - total_fields;

    printf("struct info_t at 0x%08x, size %u\n", (unsigned)(uintptr_t)&device_card, (unsigned)total_struct);
    printf("  serial   0x%08x, size %u, offset %2u, value %u\n",
           (unsigned)(uintptr_t)&device_card.serial, (unsigned)size_serial, (unsigned)off_serial, (unsigned)device_card.serial);
    printf("  version  0x%08x, size %u, offset %2u, value %u\n",
           (unsigned)(uintptr_t)&device_card.version, (unsigned)size_version, (unsigned)off_version, (unsigned)device_card.version);
    printf("  name     0x%08x, size %u, offset %2u, value %s\n",
           (unsigned)(uintptr_t)&device_card.name, (unsigned)size_name, (unsigned)off_name, device_card.name);

    printf("fields %u, sizeof %u, padding %u\n", (unsigned)total_fields, (unsigned)total_struct, (unsigned)padding);
}