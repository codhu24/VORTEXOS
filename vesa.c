#include "vesa.h"
#include <stdint.h>

struct framebuffer fb;

// Multiboot info structure
struct multiboot_info {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t syms[4];
    uint32_t mmap_length;
    uint32_t mmap_addr;
    uint32_t drives_length;
    uint32_t drives_addr;
    uint32_t config_table;
    uint32_t boot_loader_name;
    uint32_t apm_table;
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;
    // These are the crucial fields for graphics mode
    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t framebuffer_bpp;
    uint8_t framebuffer_type;
} __attribute__((packed));
void write_string1(int color, const char* str) {
    volatile char* video_memory = (volatile char*)0xB8000;
    while(*str) {
        *video_memory = *str;
        video_memory++;
        *video_memory = color;
        video_memory++;
        str++;
    }
}


// Simple 8x8 font - only characters we need
// In vesa.c, replace your font array with this one:
static const uint8_t font[128][8] = {
    [' '] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, // Space
    ['!'] = {0x18,0x18,0x18,0x18,0x00,0x18,0x18,0x00}, // !
    ['B'] = {0x7C,0x66,0x66,0x7C,0x66,0x66,0x7C,0x00}, // B
    ['C'] = {0x3C,0x66,0x60,0x60,0x60,0x66,0x3C,0x00}, // C
    ['E'] = {0x7E,0x60,0x60,0x7C,0x60,0x60,0x7E,0x00}, // E
    ['G'] = {0x3C,0x66,0x60,0x6E,0x66,0x66,0x3C,0x00}, // G
    ['H'] = {0x66,0x66,0x66,0x7E,0x66,0x66,0x66,0x00}, // H
    ['I'] = {0x7E,0x18,0x18,0x18,0x18,0x18,0x7E,0x00}, // I
    ['K'] = {0x66,0x6C,0x78,0x70,0x78,0x6C,0x66,0x00}, // K
    ['L'] = {0x60,0x60,0x60,0x60,0x60,0x60,0x7E,0x00}, // L
    ['N'] = {0x66,0x76,0x7E,0x7E,0x6E,0x66,0x66,0x00}, // N
    ['O'] = {0x3C,0x66,0x66,0x66,0x66,0x66,0x3C,0x00}, // O
    ['P'] = {0x7C,0x66,0x66,0x7C,0x60,0x60,0x60,0x00}, // P
    ['R'] = {0x7C,0x66,0x66,0x7C,0x78,0x6C,0x66,0x00}, // R
    ['S'] = {0x3C,0x66,0x60,0x3C,0x06,0x66,0x3C,0x00}, // S
    ['T'] = {0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x00}, // T
    ['U'] = {0x66,0x66,0x66,0x66,0x66,0x66,0x3C,0x00}, // U
    ['V'] = {0x66,0x66,0x66,0x66,0x66,0x3C,0x18,0x00}, // V
    ['e'] = {0x00,0x00,0x3C,0x06,0x3E,0x66,0x3E,0x00}, // e
    ['l'] = {0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x00}, // l
    ['o'] = {0x00,0x00,0x3C,0x66,0x66,0x66,0x3C,0x00}, // o
    ['r'] = {0x00,0x00,0x7C,0x66,0x7C,0x66,0x66,0x00}, // r
    ['t'] = {0x18,0x18,0x7E,0x18,0x18,0x18,0x0E,0x00}, // t
    ['x'] = {0x00,0x00,0x66,0x6C,0x18,0x6C,0x66,0x00}, // x
};

void set_pixel(uint32_t x, uint32_t y, uint32_t color) {
    if (x >= fb.width || y >= fb.height) return;
    if (!fb.address) return;
    
    uint32_t *pixel = (uint32_t*)(fb.address + (y * fb.pitch) + (x * 4));
    *pixel = color;
}
void fill_screen(uint32_t color) {
    if (!fb.address) return;
    
    // Calculate total number of pixels
    uint32_t total_pixels = fb.width * fb.height;
    
    // Cast framebuffer address to 32-bit pointer for efficient writing
    uint32_t *pixel = (uint32_t*)fb.address;
    
    // Fill all pixels with the specified color
    for (uint32_t i = 0; i < total_pixels; i++) {
        pixel[i] = color;
    }
}

void init_vesa(uint32_t magic, uint32_t mb_info_ptr) {
    // Cast the address to the multiboot info struct pointer
    struct multiboot_info *mbi = (struct multiboot_info *)mb_info_ptr;

    // Check flag 12 to see if the bootloader provided framebuffer info
    if (mbi->flags & (1 << 12)) {
        fb.width = mbi->framebuffer_width;
        fb.height = mbi->framebuffer_height;
        fb.bpp = mbi->framebuffer_bpp;
        fb.pitch = mbi->framebuffer_pitch;
        // The address is 64-bit, but we cast it for our 32-bit OS
        fb.address = (uint8_t *)(uintptr_t)mbi->framebuffer_addr;
    } else {
        // No framebuffer info provided, handle error
        fb.address = 0; // Set address to 0 so drawing functions fail safely
        return;
    }

    // Test: draw a simple pattern to verify it works
    
}
void draw_char(uint32_t x, uint32_t y, char c, uint32_t color) {
    if (!fb.address) return;
    // The char cast is still important
    unsigned char uc = c;
    if (uc > 127) return;
    
    // REMOVED: if (x + 8 >= fb.width || y + 8 >= fb.height) return;
    
    for (uint32_t cy = 0; cy < 8; cy++) {
        for (uint32_t cx = 0; cx < 8; cx++) {
            if (font[uc][cy] & (1 << (7 - cx))) {
                set_pixel(x + cx, y + cy, color);
            }
        }
    }
}

void draw_string(uint32_t x, uint32_t y, const char* str, uint32_t color) {
    if (!fb.address) return;
    
    uint32_t start_x = x;
    uint32_t cx = x;
    uint32_t cy = y;
    
    while (*str) {
        // If the character would go off the right edge, wrap to the next line
        if (cx + 8 >= fb.width) {
            cy += 8;      // Move down one line
            cx = start_x; // Reset to the initial x position
        }

        // If the character would go off the bottom edge, stop drawing
        if (cy + 8 >= fb.height) {
            return;
        }

        draw_char(cx, cy, *str, color);
        cx += 8; // Move to the right for the next character
        str++;
    }
}






