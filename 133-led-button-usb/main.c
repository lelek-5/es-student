#include <stdio.h>
#include "pico/stdlib.h"

#define LED_PIN 25
#define BUTTON_PIN 15

bool get_button_debounce(uint pin)
{
    static bool stable = false;
    static bool last = false;
    static absolute_time_t last_time = 0;

    bool raw = !gpio_get(pin);

    if (raw != last) {
        last_time = get_absolute_time();
        last = raw;
    }

    if (absolute_time_diff_us(last_time, get_absolute_time()) > 5000) {
        stable = raw;
    }

    return stable;
}

void set_led(bool on)
{
    gpio_put(LED_PIN, on);
    printf("LED %s\n", on ? "on" : "off");
}

bool handle_command(int command, bool led)
{
    if (command == 'e') {
        if (!led) {
            set_led(true);
        }
        return true;
    } else if (command == 'd') {
        if (led) {
            set_led(false);
        }
        return false;
    } else {
        printf("unknown command: %c\n", command);
        return led;
    }
}

int main()
{
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    bool led = false;
    bool last_button = false;

    while (1) {
        bool button = get_button_debounce(BUTTON_PIN);

        // Нажатие кнопки: меняем состояние и логируем
        if (button && !last_button) {
            led = !led;
            set_led(led);
        }
        last_button = button;

        int command = getchar_timeout_us(0);
        if (command == PICO_ERROR_TIMEOUT) {
            continue;
        }

        led = handle_command(command, led);
    }
}