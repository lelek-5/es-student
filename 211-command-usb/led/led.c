
#include "led.h"
#include "pico/stdlib.h"

static bool is_on = false;

void led_init(void) {
    gpio_init(led_pin());
    gpio_set_dir(led_pin(), GPIO_OUT);
    led_set(false);
}

void led_set(bool on) {
    gpio_put(led_pin(), on ? 1 : 0);
    is_on = on;
}

bool led_is_on(void) {
    return is_on;
}

uint32_t led_pin(void) {
    return PICO_DEFAULT_LED_PIN;
}

void led_toggle(void) {
    led_set(!led_is_on());
}