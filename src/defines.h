#include <stdint.h>
#include "chip8.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include <stdlib.h>

#define WINDOW_TITLE "CHIP 8"
#define SCREEN_WIDTH 64 << 4
#define SCREEN_HEIGHT 32 << 4
#define IMAGE_FLAGS IMG_INIT_PNG
#define TEXT_SIZE 80
#define CPU_HZ 500
#define TIMER_HZ 60 
#define CPU_STEP_MS   (1000 / CPU_HZ)     // 1.25 ms
#define TIMER_STEP_MS (1000 / TIMER_HZ)   // 16.666 ms

struct Game {
    SDL_Window *window;
    SDL_Renderer *renderer;
};

// Functions
static void renderChip8Screen(struct Game *game, cpu_registers_t *cpu);
void handleInput(cpu_registers_t* cpu, struct Game game);

bool sdl_initialize(struct Game *game);

void cleanAll(cpu_registers_t* cpu, struct Game *game, int exit_status);
void game_cleanup(struct Game *game, int exit_status);
void print_rom_hex(uint8_t *rom, size_t size);

//Input program
uint8_t *load_rom(const char *path, size_t *out_size);

//Logs 
void logFile(FILE *log_file, uint16_t opcode, cpu_registers_t *cpu);
void log_keypad(FILE *log_file, cpu_registers_t *cpu);
void log_timers(FILE *log_file, cpu_registers_t *cpu);



