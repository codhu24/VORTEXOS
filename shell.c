// #include "keyboard.h"
// #include "string.h"
// #include "vesa.h" 
// #include "shell.h"
// static char cmd_buffer[MAX_COMMAND_LENGTH];
// static int cursor_pos=0;
// static char cmd_his[MAX_HISTORY][MAX_COMMAND_LENGTH];
// static int hist_cnt=0;
// static int hist_ind=0;
// void init_shell(void)
// {
//     cmd_buffer[0]=0;
//     cursor_pos=0;
//     hist_cnt=0;
//     hist_ind=0;
// }
// void print_prompt(void)
// {
//     draw_string(10,10,"VortexOS->",0xFF000000);
// }
// void execute_command(char *input)
// {
// char *args[MAX_ARGS];
// int arg_count=0;
// char input_copy[MAX_COMMAND_LENGTH];
// mystrcpy(input_copy,input);
// char *token=input_copy;
// for(int i=0;i<MAX_ARGS-1;i++)
// {
//     //skip front spaces
//     while(*token ==' ')token++;
// if(*token=='\0')break;
// args[arg_count++]=token;
// //find end of token
// while(*token && *token !=' ')token++;
// if(*token==' ')
// {
//     *token++='\0';
// }
// }
// args[arg_count]=NULL;
// if(arg_count==0)return;
// //add hoistory
// if(hist_cnt<MAX_HISTORY && mystrlen(input)>0)
// {
// mystrcpy(cmd_his[hist_cnt],input);
// hist_cnt++;
// }
// if(mystrcmp(args[0],"help")==0)
// {
// draw_string(10, 30, "Available commands:", 0xFF000000);
//         draw_string(10, 45, "  help    - Show this help", 0xFF000000);
//         draw_string(10, 60, "  clear   - Clear screen", 0xFF000000);
//         draw_string(10, 75, "  echo    - Print text", 0xFF000000);
//         draw_string(10, 90, "  info    - System information", 0xFF000000);
//         draw_string(10, 105, "  reboot  - Reboot system", 0xFF000000);
// }
// else if(mystrcmp(args[0],"clear")==0)
// {
// fill_screen(0x00FFFFFF);
// }
// else if(mystrcmp(args[0],"echo")==0)
// {
// char output[MAX_COMMAND_LENGTH]="";
// for(int i=0;i<arg_count;i++)
// {
//     mystrcat(output,args[i]);
//     if(i<arg_count-1)
//     {
//         mystrcat(output," ");
//     }
//     draw_string(10,30,output,0xFF000000);

// }

// }
// else if(mystrcmp(args[0],"info")==0)
// {
// draw_string(10, 30, "VortexOS v0.1 - Simple x86 Operating System", 0xFF000000);
//         draw_string(10, 45, "Framebuffer: 1024x768x32", 0xFF000000);
//         draw_string(10, 60, "Shell: Basic command interpreter", 0xFF000000);
// }
// // else if(mystrcmp(args[0],"reboot")==0)
// // {
// // draw_string(10, 30, "System rebooting...", 0xFFFF0000);
// //         // Trigger reboot through keyboard controller
// //         asm volatile ("cli");
// //         uint8_t good = 0x02;
// //         while (good & 0x02) {
// //             good = inb(0x64);
// //         }
// //         outb(0x64, 0xFE);
// //         asm volatile ("hlt");
// // }
// else
// {
//  char error_msg[64];
//         mystrcpy(error_msg, "Unknown command: ");
//         mystrcat(error_msg, args[0]);
//         draw_string(10, 30, error_msg, 0xFFFF0000); // Red text for errors
// }
// }
// void draw_shell_icon(int x, int y)
// {
// uint32_t icon_color = 0xFF000000; // Black
// uint32_t bg_color = 0xFF00FF00;   // Green background
// //background
// for(int i=0;i<16;i++)
// {
//     for(int j=0;j<16;j++)
//     {
//         set_pixel(x+j,y+i,bg_color);
//     }
// }
// //draw terminal window outline
// for(int i=0;i<16;i++)
// {
//     set_pixel(x,y+i,icon_color);
//     set_pixel(x+15,y+i,icon_color);
// }
// for(int j=0;j<16;j++)
// {
//     set_pixel(x+j,y,icon_color);
//     set_pixel(x+j,y+15,icon_color);
// }
// //symbool
// draw_string(x+2,y+4,">",icon_color);
// draw_string(x+2,y+10,"_",icon_color);
// }
// void shell_loop(void)
// {
//     int display_y = 50;
//     init_shell();
//     fill_screen(0x00FFFFFF); // White background
//     print_prompt();
    
//     while (1) {
//         if (keyboard_has_input()) {
//             char c = keyboard_getchar();
            
//             // Handle Enter key
//             if (c == '\n') {
//                 // Execute command
//                 if (cursor_pos > 0) {
//                     draw_string(50, display_y, cmd_buffer, 0xFF000000);
//                     execute_command(cmd_buffer);
//                     display_y += 20;
                    
//                     // Clear command buffer
//                     cmd_buffer[0] = '\0';
//                     cursor_pos = 0;
//                 }
//                 print_prompt();
//             }
//             // Handle backspace
//             else if (c == 0x08 || c == 0x7F) { // Backspace or Delete
//                 if (cursor_pos > 0) {
//                     cursor_pos--;
//                     cmd_buffer[cursor_pos] = '\0';
//                     // Redraw command line to show updated text
//                     fill_rect(10 + 80, 10, 200, 16, 0x00FFFFFF); // Clear area
//                     draw_string(10 + 80, 10, cmd_buffer, 0xFF000000);
//                 }
//             }
//             // Handle escape (return to dashboard)
//             else if (c == 0x1B) { // ESC key
//                 return; // Exit shell and return to dashboard
//             }
//             // Normal character input
//             else if (c >= 32 && c <= 126 && cursor_pos < MAX_COMMAND_LENGTH - 1) {
//                 cmd_buffer[cursor_pos] = c;
//                 cursor_pos++;
//                 cmd_buffer[cursor_pos] = '\0';
//                 // Display current command
//                 fill_rect(10 + 80, 10, 200, 16, 0x00FFFFFF); // Clear area
//                 draw_string(10 + 80, 10, cmd_buffer, 0xFF000000);
//             }
//         }
//     }
// }
#include "keyboard.h"
#include "string.h"
#include "vesa.h" 
#include "shell.h"

static char cmd_buffer[MAX_COMMAND_LENGTH];
static int cursor_pos = 0;
static char cmd_his[MAX_HISTORY][MAX_COMMAND_LENGTH];
static int hist_cnt = 0;
static int hist_ind = 0;
static int current_line = 0;  // Track current line position

void init_shell(void) {
    cmd_buffer[0] = 0;
    cursor_pos = 0;
    hist_cnt = 0;
    hist_ind = 0;
    current_line = 1;  // Start from line 1 (line 0 is for prompt)
}

void print_prompt(void) {
    // Clear the prompt line area
    fill_rect(0, current_line * 20, fb.width, 20, 0x00FFFFFF);
    draw_string(10, current_line * 20, "VortexOS-> ", 0xFF000000);
    
    // Display current command buffer
    draw_string(10 + 80, current_line * 20, cmd_buffer, 0xFF000000);
}

void execute_command(char *input) {
    char *args[MAX_ARGS];
    int arg_count = 0;
    char input_copy[MAX_COMMAND_LENGTH];
    mystrcpy(input_copy, input);
    char *token = input_copy;
    
    for(int i = 0; i < MAX_ARGS - 1; i++) {
        // Skip front spaces
        while(*token == ' ') token++;
        if(*token == '\0') break;
        args[arg_count++] = token;
        // Find end of token
        while(*token && *token != ' ') token++;
        if(*token == ' ') {
            *token++ = '\0';
        }
    }
    args[arg_count] = NULL;
    if(arg_count == 0) return;
    
    // Add to history
    if(hist_cnt < MAX_HISTORY && mystrlen(input) > 0) {
        mystrcpy(cmd_his[hist_cnt], input);
        hist_cnt++;
    }
    
    if(mystrcmp(args[0], "help") == 0) {
        draw_string(10, (current_line + 1) * 20, "Available commands:", 0xFF000000);
        draw_string(10, (current_line + 2) * 20, "  help    - Show this help", 0xFF000000);
        draw_string(10, (current_line + 3) * 20, "  clear   - Clear screen", 0xFF000000);
        draw_string(10, (current_line + 4) * 20, "  echo    - Print text", 0xFF000000);
        draw_string(10, (current_line + 5) * 20, "  info    - System information", 0xFF000000);
        draw_string(10, (current_line + 6) * 20, "  reboot  - Reboot system", 0xFF000000);
        current_line += 7;
    }
    else if(mystrcmp(args[0], "clear") == 0) {
        fill_screen(0x00FFFFFF);
        current_line = 1;
    }
    else if(mystrcmp(args[0], "echo") == 0) {
        char output[MAX_COMMAND_LENGTH] = "";
        for(int i = 1; i < arg_count; i++) {  // Start from 1 to skip "echo"
            mystrcat(output, args[i]);
            if(i < arg_count - 1) {
                mystrcat(output, " ");
            }
        }
        draw_string(10, (current_line + 1) * 20, output, 0xFF000000);
        current_line += 2;
    }
    else if(mystrcmp(args[0], "info") == 0) {
        draw_string(10, (current_line + 1) * 20, "VortexOS v0.1 - Simple x86 Operating System", 0xFF000000);
        draw_string(10, (current_line + 2) * 20, "Framebuffer: 1024x768x32", 0xFF000000);
        draw_string(10, (current_line + 3) * 20, "Shell: Basic command interpreter", 0xFF000000);
        current_line += 4;
    }
    else {
        char error_msg[64];
        mystrcpy(error_msg, "Unknown command: ");
        mystrcat(error_msg, args[0]);
        draw_string(10, (current_line + 1) * 20, error_msg, 0xFFFF0000);
        current_line += 2;
    }
    
    // Check if we're running out of screen space
    if(current_line * 20 > fb.height - 40) {
        // Simple scroll - just reset to top
        fill_screen(0x00FFFFFF);
        current_line = 1;
    }
}

void draw_shell_icon(int x, int y) {
    uint32_t icon_color = 0xFF000000; // Black
    uint32_t bg_color = 0xFF00FF00;   // Green background
    
    // Background
    for(int i = 0; i < 16; i++) {
        for(int j = 0; j < 16; j++) {
            set_pixel(x + j, y + i, bg_color);
        }
    }
    
    // Draw terminal window outline
    for(int i = 0; i < 16; i++) {
        set_pixel(x, y + i, icon_color);
        set_pixel(x + 15, y + i, icon_color);
    }
    for(int j = 0; j < 16; j++) {
        set_pixel(x + j, y, icon_color);
        set_pixel(x + j, y + 15, icon_color);
    }
    
    // Symbol
    draw_string(x + 2, y + 4, ">", icon_color);
    draw_string(x + 2, y + 10, "_", icon_color);
}

void shell_loop(void) {
    init_shell();
    fill_screen(0x00FFFFFF); // White background
    print_prompt();
    // In shell_loop() function, replace the input handling section with this:

while (1) {
    if (keyboard_has_input()) {
        char c = keyboard_getchar();
        
        // DEBUG: Print the character code we received
        char debug_msg[64];
        mystrcpy(debug_msg, "Char: ");
        // Convert char to string representation
        if (c == '\n') {
            mystrcat(debug_msg, "ENTER");
        } else if (c == '\b') {
            mystrcat(debug_msg, "BACKSPACE");
        } else if (c == 0x1B) {
            mystrcat(debug_msg, "ESC");
        } else if (c < 32 || c == 127) {
            // Non-printable character, show hex code
            char hex[8];
            hex[0] = '0';
            hex[1] = 'x';
            // Simple hex conversion for debugging
            uint8_t val = (uint8_t)c;
            char nib1 = (val >> 4) & 0xF;
            char nib2 = val & 0xF;
            hex[2] = nib1 < 10 ? '0' + nib1 : 'A' + nib1 - 10;
            hex[3] = nib2 < 10 ? '0' + nib2 : 'A' + nib2 - 10;
            hex[4] = '\0';
            mystrcat(debug_msg, hex);
        } else {
            // Printable character
            char char_str[2] = {c, '\0'};
            mystrcat(debug_msg, char_str);
        }
        
        // Display debug info
        fill_rect(400, 400, 300, 20, 0x00FFFFFF);
        draw_string(400, 400, debug_msg, 0xFFFF0000);
        
        // Handle Enter key
        if (c == '\n') {
            // Execute command
            if (cursor_pos > 0) {
                // Display the executed command on a new line
                draw_string(10, (current_line + 1) * 20, cmd_buffer, 0xFF000000);
                execute_command(cmd_buffer);
                
                // Clear command buffer and reset cursor
                cmd_buffer[0] = '\0';
                cursor_pos = 0;
                
                // Move to next line for new prompt
                current_line++;
                if(current_line * 20 > fb.height - 40) {
                    fill_screen(0x00FFFFFF);
                    current_line = 1;
                }
            }
            print_prompt();
        }
        // Handle backspace - try all possible backspace codes
        else if (c == '\b' || c == 127 || c == 0x08 || c == 0x7F) {
            fill_rect(400, 420, 300, 20, 0x00FFFFFF);
            draw_string(400, 420, "Backspace detected!", 0xFFFF0000);
            
            if (cursor_pos > 0) {
                cursor_pos--;
                cmd_buffer[cursor_pos] = '\0';
                // Redraw command line to show updated text
                fill_rect(10 + 80, current_line * 20, 200, 20, 0x00FFFFFF);
                draw_string(10 + 80, current_line * 20, cmd_buffer, 0xFF000000);
            }
        }
        // Handle escape (return to dashboard)
        else if (c == 0x1B) { // ESC key
            return; // Exit shell and return to dashboard
        }
        // Normal character input
        else if (c >= 32 && c <= 126 && cursor_pos < MAX_COMMAND_LENGTH - 1) {
            cmd_buffer[cursor_pos] = c;
            cursor_pos++;
            cmd_buffer[cursor_pos] = '\0';
            // Display current command
            fill_rect(10 + 80, current_line * 20, 200, 20, 0x00FFFFFF);
            draw_string(10 + 80, current_line * 20, cmd_buffer, 0xFF000000);
        }
    }
}
//     while (1) {
//         if (keyboard_has_input()) {
//             char c = keyboard_getchar();
            
//             // Handle Enter key
//            // Handle Enter key
// if (c == '\n') {
//     // Execute command
//     if (cursor_pos > 0) {
//         // Display the executed command on a new line
//         draw_string(10, (current_line + 1) * 20, cmd_buffer, 0xFF000000);
//         execute_command(cmd_buffer);
        
//         // Clear command buffer and reset cursor
//         cmd_buffer[0] = '\0';
//         cursor_pos = 0;
        
//         // Move to next line for new prompt
//         current_line++;
//         if(current_line * 20 > fb.height - 40) {
//             fill_screen(0x00FFFFFF);
//             current_line = 1;
//         }
//     }
//     print_prompt();
// }
// // Handle backspace - FIXED VERSION
// else if (c == '\b' || c == 127 || c == 0x08) {
//         draw_string(400, 400, "Backspace detected!", 0xFFFF0000);
//     if (cursor_pos > 0) {
//         cursor_pos--;
//         cmd_buffer[cursor_pos] = '\0';
//         // Redraw command line to show updated text
//         fill_rect(10 + 80, current_line * 20, 200, 20, 0x00FFFFFF);
//         draw_string(10 + 80, current_line * 20, cmd_buffer, 0xFF000000);
//     }
// }
// // Handle escape (return to dashboard)
// else if (c == 0x1B) { // ESC key
//     return; // Exit shell and return to dashboard
// }
// // Normal character input
// else if (c >= 32 && c <= 126 && cursor_pos < MAX_COMMAND_LENGTH - 1) {
//     cmd_buffer[cursor_pos] = c;
//     cursor_pos++;
//     cmd_buffer[cursor_pos] = '\0';
//     // Display current command
//     fill_rect(10 + 80, current_line * 20, 200, 20, 0x00FFFFFF);
//     draw_string(10 + 80, current_line * 20, cmd_buffer, 0xFF000000);
// }
//     }
// }
}