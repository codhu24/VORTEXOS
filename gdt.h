// gdt.h
//Defines the C structures for a GDT entry (gdt_entry) and the GDT pointer (gdt_ptr), which tells the CPU where the GDT is located in memory.
#ifndef GDT_H
#define GDT_H

#include <stdint.h>

// GDT Entry structure
struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

// GDT Pointer structure
struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void init_gdt();

#endif