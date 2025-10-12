#include "vesa.h"
#include "pic.h"
#include "interrupts.h"
#include "gdt.h"
#include "tss.h"
#include "keyboard.h"
#include "shell.h"

extern uint32_t stack_top;
void draw_dashboard(void);
int handle_dashboard_input(void);
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

void draw_dashboard(void) {
    fill_screen(0x00333333); // Dark gray background
    
    // Draw title
    draw_string(400, 50, "VortexOS Dashboard", 0xFFFFFFFF);
    
    // Draw shell icon with label
    draw_shell_icon(200, 150);
    draw_string(190, 180, "Shell", 0xFFFFFFFF);
    draw_string(185, 200, "(Press S)", 0xFFFFFF00);
    
    // Draw other placeholder icons (you can add more later)
    for (int i = 0; i < 3; i++) {
        int x = 400 + i * 120;
        // Draw simple box icons
        for (int j = 0; j < 16; j++) {
            for (int k = 0; k < 16; k++) {
                set_pixel(x + k, 150 + j, 0xFF0000FF); // Blue icons
            }
        }
       
    }
    
    // Draw instructions
    draw_string(300, 400, "Press 'S' to launch Shell", 0xFFFFFF00);
    draw_string(300, 420, "Press 'Q' to quit applications", 0xFFFFFF00);
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
    draw_dashboard();    //draw_string(300, 300, "Hello VortexOS",  0xFF000000); // White text
    fill_screen(0x00FFFFFF);
    //draw_string(300, 300, "Hello VortexOS",  0xFF000000); // White text

    // White text

    // Simple keyboard test
    int y_pos = 320;
    int x_pos=300;
    while (1) {
        if (keyboard_has_input()) {
            char c = keyboard_getchar();
            if (c == 's' || c == 'S') {
                // Launch shell when 's' is pressed or shell icon is clicked
                shell_loop();
                // If shell returns (shouldn't normally), redraw dashboard
                draw_dashboard();
            }
            else if (c == 'q' || c == 'Q') {
                // Quit - you could add other applications here
                fill_screen(0x00FFFFFF);
                draw_string(400, 384, "Press 's' for Shell or other keys for apps", 0xFF000000);
            }
        }
    }
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