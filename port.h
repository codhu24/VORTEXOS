// This file provides a simple and direct way for the CPU to communicate with hardware devices. The CPU does this through special addresses called I/O ports.

//outb and inb: This header file provides two small but vital "inline assembly" functions. outb sends a byte of data out to a specific hardware port, and inb reads a byte of data in from a port. You can see these used throughout your drivers,
// for example, to read a scancode from the keyboard's data port (0x60).
#ifndef PORTS_H
#define PORTS_H

#include <stdint.h>

// Output byte to port
static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "dN"(port));
}

// Input byte from port
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "dN"(port));
    return ret;
}
static inline void io_wait(void)
{
outb(0x80,0);
}

#endif