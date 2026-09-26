#ifndef DEVICE_H
#define DEVICE_H

#include <stdint.h>

struct info_t
{
    uint32_t version;
    char name[13];
    uint8_t revision;
};

extern struct info_t device_card;

void device_info(void);
void dev_info(void);

#endif