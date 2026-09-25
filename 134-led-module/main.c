#include <stdio.h>
#include "pico/stdlib.h"
#include "led.h"

const uint BUTTON_PIN = 15;

bool get_button_debounce(uint pin) {
    bool state = gpio_get(pin);
    sleep_ms(20);
    return state;
}

void handle_command(int command) {
    if (command == 'e') {
        led_set(true);
        printf("led %s\n", led_is_on() ? "on" : "off");
    } else if (command == 'd') {
        led_set(false);
        printf("led %s\n", led_is_on() ? "on" : "off");
    } else {
        printf("unknown command: %c\n", command);
    }
}

int main() {
    stdio_init_all();

    led_init();

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    bool previous_button = gpio_get(BUTTON_PIN);

    while (1) {
        bool current_button = get_button_debounce(BUTTON_PIN);

        if (previous_button == 1 && current_button == 0) {
            led_toggle();
            printf("led %s\n", led_is_on() ? "on" : "off");
        }
        previous_button = current_button;

        int command = getchar_timeout_us(0);

        if (command == PICO_ERROR_TIMEOUT) {
            continue;
        }

        handle_command(command);
    }

    return 0;
}