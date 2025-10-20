#include "vesa.h"
#include "pic.h"
#include "interrupts.h"
#include "gdt.h"
#include "tss.h"
#include "keyboard.h"
#include "shell.h"
#include "snakegame.h"
#include "timer.h"
#include "rmfs.h"
#include "notepad.h"
extern uint32_t stack_top;
void draw_dashboard(void);
int handle_dashboard_input(void);
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
// void draw_snake_icon(int x, int y) {
//     uint32_t snake_color = 0xFF00FF00; // Green
//     uint32_t bg_color = 0xFF003300;    // Dark green background
    
//     // Background
//     for (int i = 0; i < 16; i++) {
//         for (int j = 0; j < 16; j++) {
//             set_pixel(x + j, y + i, bg_color);
//         }
//     }
    
//     // Draw snake body
//     set_pixel(x + 4, y + 8, snake_color);
//     set_pixel(x + 5, y + 8, snake_color);
//     set_pixel(x + 6, y + 8, snake_color);
//     set_pixel(x + 7, y + 8, snake_color);
//     set_pixel(x + 8, y + 8, snake_color);
//     set_pixel(x + 8, y + 7, snake_color);
//     set_pixel(x + 8, y + 6, snake_color);
    
//     // Draw snake head
//     set_pixel(x + 9, y + 6, snake_color);
//     set_pixel(x + 10, y + 6, snake_color);
    
//     // Draw food
//     set_pixel(x + 12, y + 6, 0xFFFF0000);
// }
void draw_snake_icon(int x, int y) {
    uint32_t bg_color = 0xFF1a3a1a;    // Dark green
    uint32_t snake_color = 0xFF00FF00; // Bright green
    uint32_t food_color = 0xFFFF0000;  // Red
    
    // Game board background
    fill_rect(x, y, 48, 36, bg_color);
    
    // Snake body
    fill_rect(x + 10, y + 18, 12, 4, snake_color);
    fill_rect(x + 18, y + 14, 4, 8, snake_color);
    fill_rect(x + 22, y + 14, 8, 4, snake_color);
    
    // Snake head
    fill_rect(x + 30, y + 14, 6, 4, 0xFF80FF80); // Lighter green for head
    
    // Food
    fill_rect(x + 35, y + 8, 4, 4, food_color);
}

void draw_system_icon(int x, int y) {
    uint32_t bg_color = 0xFF3a1a3a;    // Dark purple
    uint32_t accent_color = 0xFFFFA500; // Orange
    
    // Computer tower
    fill_rect(x + 15, y + 8, 18, 28, bg_color);
    
    // Details
    fill_rect(x + 18, y + 12, 12, 4, accent_color);  // LED
    fill_rect(x + 17, y + 20, 14, 2, 0xFF666666);    // Drive slot
    fill_rect(x + 17, y + 25, 14, 2, 0xFF666666);    // Drive slot
    
    // Monitor
    fill_rect(x + 8, y + 5, 32, 20, 0xFF111111);
    fill_rect(x + 10, y + 7, 28, 16, 0xFF0066CC);
    
    // Stand
    fill_rect(x + 21, y + 25, 6, 8, 0xFF444444);
}
void draw_dashboard(void) {
    fill_screen(0x00333333); // Dark gray background
    
    // Draw title
    draw_string(400, 50, "VortexOS Dashboard", 0xFFFFFFFF);
    
    // Draw shell icon with label
    draw_shell_icon(200, 150);
    draw_string(190, 180, "Shell", 0xFFFFFFFF);
    draw_string(185, 200, "(Press S)", 0xFFFFFF00);
     draw_snake_icon(350, 150);
    draw_string(340, 180, "Snake Game", 0xFFFFFFFF);
    draw_string(335, 200, "(Press G)", 0xFFFFFF00);
    
    // Draw other placeholder icons (you can add more later)
   draw_notepad_icon(500, 150);
    draw_string(490, 180, "Notepad", 0xFFFFFFFF);
    draw_string(485, 200, "(Press N)", 0xFFFFFF00);
    
    // Update instructions
    draw_string(300, 400, "Press 'S' for Shell, 'G' for Snake Game, 'N' for Notepad", 0xFFFFFF00);
}

void kernel_main(uint32_t magic, uint32_t mb_info) {
    // The correct initialization order
    init_gdt();
    init_tss();
    init_idt();
    pic_remap();
        init_keyboard();
init_timer(100);
rmfs_init();
    for (int i = 0; i < 16; i++) {
        {pic_set_mask(i);}
    }
    pic_clear_mask(0);
    pic_clear_mask(1);
    // Now it's safe to enable interrupts
    asm volatile("sti");
    // Initialize display
    init_vesa(magic, mb_info);
    
    // Draw to the screen
    draw_dashboard();    //draw_string(300, 300, "Hello VortexOS",  0xFF000000); // White text

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
            else if(c=='g'||c=='G')
            {
                snake_game_loop();
                draw_dashboard();
            }
            else if(c=='N'|| c=='n')
            {
                notepad_loop("notes.txt");
                draw_dashboard();
            }
            else if (c == 'q' || c == 'Q') {
                // Quit - you could add other applications here
                fill_screen(0x00FFFFFF);
                draw_string(400, 384, "Press 's' for Shell or other keys for apps", 0xFF000000);
            }
        }
    }
}