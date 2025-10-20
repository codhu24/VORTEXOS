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
<<<<<<< HEAD
#include "colours.h"
=======
>>>>>>> d044cf8ee47b5701af43a01637930ed874f65cb6
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
<<<<<<< HEAD

=======
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
>>>>>>> d044cf8ee47b5701af43a01637930ed874f65cb6
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
<<<<<<< HEAD
=======
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
>>>>>>> d044cf8ee47b5701af43a01637930ed874f65cb6
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
    fill_rect(x + 21, y + 25, 6, 8, 0xFF444444);}

void draw_shell_icon(int x, int y) {
    uint32_t bg_color = 0xFF2a2a2a;    // Dark gray
    uint32_t text_color = 0xFF00FF00;  // Green text
    
    // Terminal window
    fill_rect(x, y, 50, 40, bg_color);
    
    // Terminal header
    fill_rect(x, y, 50, 8, 0xFF555555);
    
    // Terminal prompt
    draw_string(x + 5, y + 15, ">", text_color);
    draw_string(x + 15, y + 15, "shell", 0xFF00AAFF);
        fill_rect(x + 45, y + 15, 3, 8, text_color);
}
void draw_notepad_icon(int x, int y) {
    uint32_t paper_color = 0xFFFFFFFF;  // White paper
    uint32_t cover_color = 0xFF1E90FF;  // Dodger blue cover
        fill_rect(x, y, 50, 40, cover_color);
        fill_rect(x + 5, y + 5, 45, 35, paper_color);
        for (int i = 0; i < 4; i++) {
        fill_rect(x + 10, y + 15 + i * 6, 35, 2, 0xFFDDDDDD);
    }
        fill_rect(x, y, 3, 40, 0xFF666666);
}
void draw_icon_card(uint32_t x, uint32_t y, const char* title, uint32_t base_color, void (*draw_icon_func)(int, int)) {
    fill_rect(x-10, y-10, 140, 160, COLOR_ALMOST_BLACK);
    fill_rect(x-5, y-5, 130, 150, base_color);
    fill_rect(x, y, 120, 140, COLOR_SLATE_GRAY);
        if (draw_icon_func) {
        draw_icon_func(x + 35, y + 30); // Center the icon (120-50=70/2=35, 140-80=60/2=30)
    }
        uint32_t title_len = 0;
    const char* temp = title;
    while (*temp++) title_len++;
    uint32_t text_x = x + (120 - title_len * 8) / 2; // Center text based on character count
    draw_string_scaled(text_x, y + 105, title, COLOR_CHARCOAL,1);
}

void draw_dashboard(void) {
    for (uint32_t y = 0; y < fb.height; y++) {
        uint32_t color = interpolate_color(COLOR_NAVY, COLOR_PURPLE, y, fb.height);
                uint32_t *row_start = (uint32_t*)(fb.address + (y * fb.pitch));
                for (uint32_t x = 0; x < fb.width; x++) {
            row_start[x] = color;
        }
    }
        draw_string_scaled(225, 45, "VortexOS Dashboard", COLOR_CHARCOAL, 4);
    draw_string_scaled(230, 50, "VortexOS Dashboard", COLOR_GOLD, 4);
        draw_icon_card(260, 150, "Shell", COLOR_DIM_GRAY, draw_shell_icon);
    draw_icon_card(460, 150, "Snake Game", COLOR_SEA, draw_snake_icon);
    draw_icon_card(660, 150, "Notepad", COLOR_NAVY, draw_notepad_icon);
        draw_string_scaled(280, 470, "PRESS S=SHELL  G=GAME  N=NOTEPAD", COLOR_PASTEL_YELLOW, 2);
}
void kernel_main(uint32_t magic, uint32_t mb_info) {
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
    asm volatile("sti");
    init_vesa(magic, mb_info);
        draw_dashboard();    //draw_string(300, 300, "Hello VortexOS",  0xFF000000); // White text
    int y_pos = 320;
    int x_pos=300;
    while (1) {
        if (keyboard_has_input()) {
            char c = keyboard_getchar();
            if (c == 's' || c == 'S') {
                shell_loop();
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