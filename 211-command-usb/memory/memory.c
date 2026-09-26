#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "memory.h"
#include "led.h"
#include "device.h"

// Стандартные символы линкера Pico SDK
extern char __StackTop[];             // вершина стека (capital S, capital T!)
extern char __flash_binary_start[];   // начало образа во флеш-памяти
extern char __flash_binary_end[];     // конец образа во флеш-памяти

void mem_info(void) {
    uint32_t stack_top = (uint32_t)(uintptr_t)__StackTop;
    uint32_t flash_start = (uint32_t)(uintptr_t)__flash_binary_start;
    uint32_t flash_end = (uint32_t)(uintptr_t)__flash_binary_end;

    // RAM на RP2040: 256 КБ основной + 4 КБ SCRATCH_X + 4 КБ SCRATCH_Y
    uint32_t ram_start = 0x20000000;
    uint32_t ram_end = 0x20042000;

    printf("stack          0x%08x\n", stack_top);
    printf("flash          0x%08x - 0x%08x\n", flash_start, flash_end);
    printf("ram            0x%08x - 0x%08x\n", ram_start, ram_end);
}

void fw_info(void) {
    extern struct info_t device_card;
    extern void main(void);

    uint32_t addr_device_card = (uint32_t)(uintptr_t)&device_card;
    uint32_t addr_main = (uint32_t)(uintptr_t)main;

    printf("device_card    0x%08x %5u\n", addr_device_card, (unsigned)sizeof(device_card));
    printf("main             0x%08x\n", addr_main);
}

void boot_info(void) {
    const uint32_t *vectors = (const uint32_t *)0x10000100;
    uint32_t stack_top_val = vectors[0];
    uint32_t reset_handler = vectors[1];

    printf("vector table   0x%08x\n", 0x10000100);
    printf("  stack top    0x%08x\n", (unsigned)stack_top_val);
    printf("  reset        0x%08x\n", (unsigned)reset_handler);
    printf("  reset (even) 0x%08x\n", (unsigned)(reset_handler & ~1u));

    volatile uint32_t *gpio_in = (volatile uint32_t *)0xd0000004;
    uint32_t level = (*gpio_in >> led_pin()) & 1u;

    printf("gpio in        0x%08x\n", 0xd0000004);
    printf("  led bit      %u\n", (unsigned)level);
    printf("  gpio_get     %u\n", (unsigned)gpio_get(led_pin()));
}