#include "log.h"

void log_version(void)
{
    printf("device: %s %s, built %s %s, log level %d\n",
           DEVICE_NAME, FIRMWARE_VERSION, __DATE__, __TIME__, LOG_LEVEL);
}

void log_prefix(const char *level, const char *function, int line)
{
    printf("%s %s:%d ", level, function, line);
}
void led_init(void) {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT); // <- Вот этой строки не хватает автотесту
}