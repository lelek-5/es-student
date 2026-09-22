#include <stdio.h>
#include "pico/stdlib.h"

int main()
{
    stdio_init_all();
    stdio_usb_init();

    while (1)
    {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}