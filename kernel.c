#include "vesa.h"
#include "pic.h"
#include "interrupts.h"
#include "gdt.h"
#include "tss.h"
#include "keyboard.h"
extern uint32_t stack_top;
void write_string(int color, const char* str) {
    volatile char* video_memory = (volatile char*)0xB8000;
    while(*str) {
        *video_memory = *str;
        video_memory++;
        *video_memory = color;
        video_memory++;
        str++;
    }
}



void kernel_main(uint32_t magic, uint32_t mb_info) {
    // The correct initialization order
    init_gdt();
    init_tss();
    init_idt();
    pic_remap();
        init_keyboard();

    for (int i = 0; i < 16; i++) {
        {pic_set_mask(i);}
    }
    pic_clear_mask(1);
    // Now it's safe to enable interrupts
    asm volatile("sti");
    // Initialize display
    init_vesa(magic, mb_info);
    
    // Draw to the screen
    fill_screen(0x00FFFFFF);
    //draw_string(300, 300, "Hello VortexOS",  0xFF000000); // White text

    // White text

    // Simple keyboard test
    int y_pos = 320;
    int x_pos=300;
    while(1) {
        if (keyboard_has_input()) {
            char c = keyboard_getchar();
            if (c == '\n') {
                y_pos += 10;  // New line
            } else if (c != 0) {
                // Draw the character
                char str[2] = {c, '\0'};
                draw_string(x_pos, y_pos, str,  0xFF000000);
                x_pos+=8;
            }
        }
}
}