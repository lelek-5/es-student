#ifndef LED_H
#define LED_H

#include <stdbool.h>
#include <stdint.h>

void led_init(void);
void led_set(bool on);
bool led_is_on(void);
uint32_t led_pin(void);
void led_toggle(void);

#endif