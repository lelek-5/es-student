#include <stdio.h>
#include "pico/stdlib.h"
#include "led.h"

#define BUTTON_PIN 15

bool get_button_debounce(uint pin)
{
    static bool last_raw = false;
    static bool stable = false;
    static int count = 0;

    bool raw = !gpio_get(pin);

    if (raw == last_raw)
    {
        if (count < 3)
            count++;
        if (count >= 3)
            stable = raw;
    }
    else
    {
        count = 0;
        last_raw = raw;
    }

    return stable;
}

void handle_command(int command)
{
    if (command == 'e')
    {
        led_set(true);
        printf("led %s\n", led_is_on() ? "on" : "off");
    }
    else if (command == 'd')
    {
        led_set(false);
        printf("led %s\n", led_is_on() ? "on" : "off");
    }
    else
    {
        printf("unknown command: %c\n", command);
    }
}

int main()
{
    stdio_init_all();

    led_init();

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    bool last_button = false;

    while (1)
    {
        sleep_ms(10);

        bool button = get_button_debounce(BUTTON_PIN);

        if (button && !last_button)
        {
            led_toggle();
            printf("led %s\n", led_is_on() ? "on" : "off");
        }
        last_button = button;

        int command = getchar_timeout_us(0);

        if (command == PICO_ERROR_TIMEOUT)
        {
            continue;
        }

        handle_command(command);
    }
}