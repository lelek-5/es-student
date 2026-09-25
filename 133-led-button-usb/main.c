#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

const uint LED_PIN = 25;
const uint BUTTON_PIN = 15;

bool get_button_debounce(uint pin)
{
    static bool last_state = false;
    bool current_state = gpio_get(pin);
    sleep_ms(20);
    bool stable_state = gpio_get(pin);

    if (current_state != stable_state)
    {
        return last_state;
    }

    last_state = stable_state;
    return stable_state;
}

void set_led(bool on)
{
    gpio_put(LED_PIN, on);
    printf("led %s\n", on ? "on" : "off");
}

int main()
{
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, false);

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    bool prev_button_state = get_button_debounce(BUTTON_PIN);

    while (1)
    {
        bool current_button_state = get_button_debounce(BUTTON_PIN);

        if (current_button_state != prev_button_state)
        {
            set_led(!current_button_state);
        }

        prev_button_state = current_button_state;
    }
}