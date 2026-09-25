#include <stdio.h>
#include "pico/stdlib.h"

const uint LED_PIN = 25;       // Либо PICO_DEFAULT_LED_PIN, в зависимости от вашей платы
const uint BUTTON_PIN = 15;    // Пин, к которому подключена кнопка

bool get_button_debounce(uint pin) {
    bool state = gpio_get(pin);
    sleep_ms(20);
    return state;
}

void set_led(bool on) {
    gpio_put(LED_PIN, on);
    printf("led %s\n", on ? "on" : "off");
}

bool handle_command(int command, bool led) {
    if (command == 'e') {
        led = true;
        set_led(led);
    } else if (command == 'd') {
        led = false;
        set_led(led);
    } else {
        printf("unknown command: %c\n", command);
    }
    return led;
}

int main() {
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    bool led_state = false;
    bool last_button_state = gpio_get(BUTTON_PIN);

    while (1) {
        // 1. Опрос кнопки с антидребезгом
        bool current_button_state = get_button_debounce(BUTTON_PIN);

        // Кнопка подтянута к питанию: нажатие — переход из 1 в 0
        if (last_button_state == 1 && current_button_state == 0) {
            led_state = !led_state;
            set_led(led_state);
        }
        last_button_state = current_button_state;

        // 2. Чтение команды из USB (не блокирует цикл)
        int command = getchar_timeout_us(0);

        if (command == PICO_ERROR_TIMEOUT) {
            continue;
        }

        // Если пришёл символ — передаём в обработчик
        led_state = handle_command(command, led_state);
    }

    return 0;
}