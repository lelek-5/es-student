#include "memory.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/regs/addressmap.h"
#include "device.h"
#include "command.h"

extern char __flash_binary_start;
extern char __flash_binary_end;
extern char __boot2_start__;
extern char __boot2_end__;
extern char __etext;
extern char __data_start__;
extern char __data_end__;
extern char __bss_start__;
extern char __bss_end__;
extern char __HeapLimit;
extern char __StackBottom;
extern char __StackTop;

int main(void);

uint32_t data_variable = 100;
uint32_t bss_variable;

static void row(const char *name, uintptr_t start, uintptr_t end) {
    printf("%-10s 0x%08x 0x%08x %8u\n",
           name, (unsigned)start, (unsigned)end, (unsigned)(end - start));
}

void mem_info(void) {
    uintptr_t flash_start = XIP_BASE;
    uintptr_t flash_end = XIP_BASE + PICO_FLASH_SIZE_BYTES;

    uintptr_t sram_start = SRAM_BASE;
    uintptr_t sram_end = SRAM_BASE + 264 * 1024;

    uintptr_t rom_start = ROM_BASE;
    uintptr_t rom_end = ROM_BASE + 16 * 1024;

    uintptr_t img_start = (uintptr_t)&__flash_binary_start;
    uintptr_t img_end = (uintptr_t)&__flash_binary_end;

    uintptr_t b2_start = (uintptr_t)&__boot2_start__;
    uintptr_t b2_end = (uintptr_t)&__boot2_end__;

    uintptr_t txt_start = (uintptr_t)&__boot2_end__;
    uintptr_t txt_end = (uintptr_t)&__etext;

    uintptr_t dt_ram_start = (uintptr_t)&__data_start__;
    uintptr_t dt_ram_end = (uintptr_t)&__data_end__;
    uintptr_t dt_size = dt_ram_end - dt_ram_start;

    uintptr_t dt_fl_start = (uintptr_t)&__etext;
    uintptr_t dt_fl_end = dt_fl_start + dt_size;

    uintptr_t bss_st = (uintptr_t)&__bss_start__;
    uintptr_t bss_en = (uintptr_t)&__bss_end__;

    uintptr_t hp_st = (uintptr_t)&__bss_end__;
    uintptr_t hp_en = (uintptr_t)&__HeapLimit;

    uintptr_t st_st = (uintptr_t)&__StackBottom;
    uintptr_t st_en = (uintptr_t)&__StackTop;

    printf("%-10s %-10s %-10s %8s\n", "area", "start", "end", "size");
    row("flash", flash_start, flash_end);
    row("sram", sram_start, sram_end);
    row("rom", rom_start, rom_end);
    row("image", img_start, img_end);
    row("free", img_end, flash_end);
    row("boot2", b2_start, b2_end);
    row("text", txt_start, txt_end);
    row("data flash", dt_fl_start, dt_fl_end);
    row("data ram", dt_ram_start, dt_ram_end);
    row("bss", bss_st, bss_en);
    row("heap", hp_st, hp_en);
    row("stack", st_st, st_en);

    printf("\ntotal\n");
    printf("  flash image %8u = boot2 %u + text %u + data %u\n",
           (unsigned)(img_end - img_start),
           (unsigned)(b2_end - b2_start),
           (unsigned)(txt_end - txt_start),
           (unsigned)dt_size);
    printf("  flash free  %8u of %u\n",
           (unsigned)(flash_end - img_end),
           (unsigned)(flash_end - flash_start));
    printf("  ram used    %8u = data %u + bss %u\n",
           (unsigned)(dt_size + (bss_en - bss_st)),
           (unsigned)dt_size,
           (unsigned)(bss_en - bss_st));
    printf("  ram free    %8u for heap and %u for stack\n",
           (unsigned)(hp_en - hp_st),
           (unsigned)(st_en - st_st));
}

void fw_info(void) {
    data_variable++;
    bss_variable++;

    uint16_t *main_code = (uint16_t *)((uintptr_t)main & ~1u);
    uint16_t *fw_code = (uint16_t *)((uintptr_t)fw_info & ~1u);

    uint32_t stack_variable = 1946;
    uint32_t *heap_variable = (uint32_t *)malloc(sizeof(uint32_t));
    if (heap_variable != NULL) {
        *heap_variable = 1951;
    }

    printf("%-15s %-11s %s\n", "object", "address", "value");
    printf("%-15s 0x%08x  0x%04x\n", "main", (unsigned)(uintptr_t)main, (unsigned)*main_code);
    printf("%-15s 0x%08x  0x%04x\n", "fw_info", (unsigned)(uintptr_t)fw_info, (unsigned)*fw_code);

    printf("%-15s 0x%08x\n", "commands", (unsigned)(uintptr_t)commands);
    for (uint i = 0; i < command_count; i++) {
        char item_name[32];
        snprintf(item_name, sizeof(item_name), "- %s", commands[i].name);
        printf("%-15s 0x%08x\n", item_name, (unsigned)(uintptr_t)commands[i].handler);
    }

    printf("%-15s 0x%08x  %s\n", "DEVICE_PROJECT", (unsigned)(uintptr_t)DEVICE_PROJECT, DEVICE_PROJECT);
    printf("%-15s 0x%08x  %s\n", "DEVICE_BOARD", (unsigned)(uintptr_t)DEVICE_BOARD, DEVICE_BOARD);
    printf("%-15s 0x%08x  %u\n", "data_variable", (unsigned)(uintptr_t)&data_variable, (unsigned)data_variable);
    printf("%-15s 0x%08x  %u\n", "bss_variable", (unsigned)(uintptr_t)&bss_variable, (unsigned)bss_variable);
    printf("%-15s 0x%08x  %u\n", "stack_variable", (unsigned)(uintptr_t)&stack_variable, (unsigned)stack_variable);

    if (heap_variable != NULL) {
        printf("%-15s 0x%08x  %u\n", "heap_variable", (unsigned)(uintptr_t)heap_variable, (unsigned)*heap_variable);
        free(heap_variable);
    }
}