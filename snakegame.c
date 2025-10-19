#include "keyboard.h"
#include "vesa.h"
#include "snakegame.h"
#include "string.h"
#include "colours.h"
#include "timer.h"  // Add this include

// Remove the static system_ticks variable - we'll use the one from timer.c

void snake_init(snake_game_t *game)
{
    game->length = 3;
    game->body[0].x = GRID_WIDTH / 2;
    game->body[0].y = GRID_HEIGHT / 2;
    game->body[1].x = GRID_WIDTH / 2 - 1;
    game->body[1].y = GRID_HEIGHT / 2;
    game->body[2].x = GRID_WIDTH / 2 - 2;
    game->body[2].y = GRID_HEIGHT / 2;
    
    game->direction = DIRECTION_RIGHT;
    game->score = 0;
    game->game_over = 0;
    game->speed = 8;  // Slower initial speed for better gameplay
    game->ticks = 0;
    game->needs_redraw = 1;  // Force initial draw
    
    snake_place_food(game);
}

void snake_update(snake_game_t *game) {
    if (game->game_over) {
        game->needs_redraw = 1;  // Redraw to show game over screen
        return;
    }
    
    game->ticks++;
    if (game->ticks < game->speed) return;
    game->ticks = 0;
    
    // Move snake body (from tail to head)
    for (int i = game->length - 1; i > 0; i--) {
        game->body[i] = game->body[i - 1];
    }
    
    // Move head based on direction
    switch (game->direction) {
        case DIRECTION_UP:
            game->body[0].y--;
            break;
        case DIRECTION_DOWN:
            game->body[0].y++;
            break;
        case DIRECTION_LEFT:
            game->body[0].x--;
            break;
        case DIRECTION_RIGHT:
            game->body[0].x++;
            break;
    }
    
    // Check for collisions
    if (snake_check_collision(game)) {
        game->game_over = 1;
        game->needs_redraw = 1;
        return;
    }
    
    // Check if snake ate food
    if (game->body[0].x == game->food.x && game->body[0].y == game->food.y) {
        // Increase snake length
        if (game->length < SNAKE_MAX_LENGTH) {
            game->length++;
            // New segment starts at tail position
            game->body[game->length - 1] = game->body[game->length - 2];
        }
        
        game->score += 10;
        
        // Increase speed slightly (but not too much)
        if (game->speed > 4 && game->score % 50 == 0) {
            game->speed--;
        }
        
        snake_place_food(game);
        game->needs_redraw = 1;
    } else {
        game->needs_redraw = 1;  // Redraw on movement
    }
}

int snake_check_collision(snake_game_t *game) {
    point_t head = game->body[0];
    
    // Wall collision (more strict boundaries)
    if (head.x < 1 || head.x >= GRID_WIDTH - 1 || 
        head.y < 1 || head.y >= GRID_HEIGHT - 1) {
        return 1;
    }
    
    // Self collision (skip head)
    for (int i = 1; i < game->length; i++) {
        if (head.x == game->body[i].x && head.y == game->body[i].y) {
            return 1;
        }
    }
    
    return 0;
}

// void snake_place_food(snake_game_t *game)
// {
//     int valid_pos = 0;
//     int attempts = 0;
//     const int max_attempts = 100; // Prevent infinite loop
    
//     while(!valid_pos && attempts < max_attempts) {
//         // Ensure food spawns within playable area (not on borders)
//         game->food.x = 1 + (get_system_ticks() % (GRID_WIDTH - 2));
//         game->food.y = 1 + ((get_system_ticks() * 7) % (GRID_HEIGHT - 2));
        
//         valid_pos = 1;
        
//         // Check if food overlaps with snake
//         for (int i = 0; i < game->length; i++) {
//             if (game->food.x == game->body[i].x && game->food.y == game->body[i].y) {
//                 valid_pos = 0;
//                 break;
//             }
//         }
        
//         attempts++;
//     }
    
//     // If we couldn't find a valid position after many attempts, try a simple fallback
//     if (!valid_pos) {
//         game->food.x = GRID_WIDTH / 2;
//         game->food.y = GRID_HEIGHT / 2;
//     }
// }
void snake_place_food(snake_game_t *game)
{
    int valid_pos = 0;
    int attempts = 0;
    const int max_attempts = 200; // Increased attempts
    
    // Simple pseudo-random number generator using timer ticks
    static uint32_t seed = 0;
    if (seed == 0) {
        seed = get_system_ticks();
    }
    
    while(!valid_pos && attempts < max_attempts) {
        // Better random number generation
        seed = (seed * 1103515245 + 12345) & 0x7FFFFFFF;
        
        // Ensure food spawns within playable area (not on borders)
        // Use modulo with proper range to avoid border positions
        game->food.x = 1 + (seed % (GRID_WIDTH - 2));
        game->food.y = 1 + ((seed * 7) % (GRID_HEIGHT - 2));
        
        valid_pos = 1;
        
        // Check if food overlaps with snake body
        for (int i = 0; i < game->length; i++) {
            if (game->food.x == game->body[i].x && game->food.y == game->body[i].y) {
                valid_pos = 0;
                break;
            }
        }
        
        // Also check if food is too close to walls
        if (game->food.x <= 0 || game->food.x >= GRID_WIDTH - 1 ||
            game->food.y <= 0 || game->food.y >= GRID_HEIGHT - 1) {
            valid_pos = 0;
        }
        
        attempts++;
    }
    
    // If we couldn't find a valid position, try a systematic search
    if (!valid_pos) {
        valid_pos = 0;
        for (int y = 1; y < GRID_HEIGHT - 1 && !valid_pos; y++) {
            for (int x = 1; x < GRID_WIDTH - 1 && !valid_pos; x++) {
                game->food.x = x;
                game->food.y = y;
                
                valid_pos = 1;
                for (int i = 0; i < game->length; i++) {
                    if (game->food.x == game->body[i].x && game->food.y == game->body[i].y) {
                        valid_pos = 0;
                        break;
                    }
                }
                
                if (valid_pos) break;
            }
        }
    }
    
    // If still no valid position, place food at a default safe location
    if (!valid_pos) {
        game->food.x = GRID_WIDTH / 2;
        game->food.y = GRID_HEIGHT / 2;
        
        // Make sure default position doesn't overlap with snake
        for (int i = 0; i < game->length; i++) {
            if (game->food.x == game->body[i].x && game->food.y == game->body[i].y) {
                // If even the default position is occupied, try adjacent positions
                if (game->food.x + 1 < GRID_WIDTH - 1) {
                    game->food.x++;
                } else if (game->food.y + 1 < GRID_HEIGHT - 1) {
                    game->food.y++;
                }
                break;
            }
        }
    }
}
void snake_draw_borders(void) {
    uint32_t border_color = 0xFF00FF00; // Green
    
    // Draw thicker borders for better visibility
    // Top border
    fill_rect(0, 0, GRID_WIDTH * CELL_SIZE, 2, border_color);
    // Bottom border  
    fill_rect(0, GRID_HEIGHT * CELL_SIZE - 2, GRID_WIDTH * CELL_SIZE, 2, border_color);
    // Left border
    fill_rect(0, 0, 2, GRID_HEIGHT * CELL_SIZE, border_color);
    // Right border
    fill_rect(GRID_WIDTH * CELL_SIZE - 2, 0, 2, GRID_HEIGHT * CELL_SIZE, border_color);
}

void snake_draw(snake_game_t *game)
{
    // Only clear the game area, not the entire screen
    fill_rect(2, 2, GRID_WIDTH * CELL_SIZE - 4, GRID_HEIGHT * CELL_SIZE - 4, COLOR_BLACK);

    snake_draw_borders();
    
    // Draw snake
    for(int i = 0; i < game->length; i++) {
        uint32_t color = (i == 0) ? 0xFF00FF00 : 0xFF008800; // Head is brighter green
        fill_rect(game->body[i].x * CELL_SIZE, 
                  game->body[i].y * CELL_SIZE, 
                  CELL_SIZE - 1, CELL_SIZE - 1, color);
    }
    
    // Draw food
    fill_rect(game->food.x * CELL_SIZE, 
              game->food.y * CELL_SIZE, 
              CELL_SIZE - 1, CELL_SIZE - 1, COLOR_RED);
    
    // Draw score
    char score_text[32];
    mystrcpy(score_text, "Score: ");
    char score_num[16];
    int score = game->score;
    int pos = 0;
    
    if (score == 0) {
        score_num[pos++] = '0';
    } else {
        char temp[16];
        int temp_pos = 0;
        while (score > 0) {
            temp[temp_pos++] = '0' + (score % 10);
            score /= 10;
        }
        for (int i = temp_pos - 1; i >= 0; i--) {
            score_num[pos++] = temp[i];
        }
    }
    score_num[pos] = '\0';
    mystrcat(score_text, score_num);
    
    // Clear previous score area
    fill_rect(10, GRID_HEIGHT * CELL_SIZE + 5, 200, 30, COLOR_BLACK);
    draw_string(10, GRID_HEIGHT * CELL_SIZE + 10, score_text, 0xFFFFFFFF);
    
    if (game->game_over) {
        draw_string(GRID_WIDTH * CELL_SIZE / 2 - 80, GRID_HEIGHT * CELL_SIZE / 2 - 10, 
                   "GAME OVER!", 0xFFFF0000);
        draw_string(GRID_WIDTH * CELL_SIZE / 2 - 100, GRID_HEIGHT * CELL_SIZE / 2 + 10, 
                   "Press R to restart", 0xFFFFFF00);
        draw_string(GRID_WIDTH * CELL_SIZE / 2 - 100, GRID_HEIGHT * CELL_SIZE / 2 + 30, 
                   "Press ESC to exit", 0xFFFFFF00);
    } else {
        // Draw controls hint
        draw_string(10, GRID_HEIGHT * CELL_SIZE + 30, "WASD to move, ESC to exit", 0xFFFFFF00);
    }
    
    game->needs_redraw = 0;  // Reset redraw flag
}

void snake_handle_input(snake_game_t *game, char key) {
    if (game->game_over) {
        if (key == 'r' || key == 'R') {
            snake_init(game);
            game->needs_redraw = 1;
        }
        return;
    }
    
    switch (key) {
        case 'w': case 'W':
            if (game->direction != DIRECTION_DOWN)
                game->direction = DIRECTION_UP;
            break;
        case 's': case 'S':
            if (game->direction != DIRECTION_UP)
                game->direction = DIRECTION_DOWN;
            break;
        case 'a': case 'A':
            if (game->direction != DIRECTION_RIGHT)
                game->direction = DIRECTION_LEFT;
            break;
        case 'd': case 'D':
            if (game->direction != DIRECTION_LEFT)
                game->direction = DIRECTION_RIGHT;
            break;
    }
}

void snake_game_loop(void)
{
    snake_game_t game;
    snake_init(&game);
    
    // Center the game on screen
    int x = (fb.width - GRID_WIDTH * CELL_SIZE) / 2;
    int y = (fb.height - GRID_HEIGHT * CELL_SIZE) / 2 - 50;
    
    // Clear screen only once at start
    fill_screen(COLOR_BLACK);
    
    while(1) {
        // Handle input
        if(keyboard_has_input()) {
            char c = keyboard_getchar();
            if(c == 0x1B) { // ESC
                break;
            }
            snake_handle_input(&game, c);
        }
        
        // Update game state
        snake_update(&game);
        
        // Only redraw if something changed
        if (game.needs_redraw) {
            snake_draw(&game);
        }
        
        // Use HLT to wait for next interrupt (timer or keyboard)
        // This saves CPU cycles and makes the game smooth
        asm volatile("hlt");
    }
}