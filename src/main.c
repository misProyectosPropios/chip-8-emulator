#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include "chip8.h"

#define WINDOW_TITLE "05 Create Text"
#define SCREEN_WIDTH 64 << 4
#define SCREEN_HEIGHT 32 << 4
#define IMAGE_FLAGS IMG_INIT_PNG
#define TEXT_SIZE 80


struct Game {
    SDL_Window *window;
    SDL_Renderer *renderer;
};

void game_cleanup(struct Game *game, int exit_status);
bool sdl_initialize(struct Game *game);
void cleanAll(cpu_registers_t* cpu, struct Game *game, int exit_status);

static void renderChip8Screen(struct Game *game, cpu_registers_t *cpu) {
    int scale = SCREEN_WIDTH / CHIP8_SCREEN_WIDTH;
    SDL_Rect rect;
    rect.w = scale;
    rect.h = scale;

    for (int y = 0; y < CHIP8_SCREEN_HEIGHT; y++) {
        for (int x = 0; x < CHIP8_SCREEN_WIDTH; x++) {
            uint8_t pixel = cpu->display[y][x];
            if (pixel) {
                SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
            } else {
                SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
            }
            rect.x = x * scale;
            rect.y = y * scale;
            SDL_RenderFillRect(game->renderer, &rect);
        }
    }
}

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint8_t *load_rom(const char *path, size_t *out_size) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        perror("fopen");
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    if (size <= 0) {
        fclose(f);
        return NULL;
    }

    uint8_t *buffer = malloc(size);
    if (!buffer) {
        fclose(f);
        return NULL;
    }

    fread(buffer, 1, size, f);
    fclose(f);

    *out_size = size;
    return buffer;
}

void loadProgram(cpu_registers_t *cpu, uint8_t *program, size_t size) {
    for(size_t i = 0; i < size; i++) {
        cpu->memory[i + 0x200] = program[i];
    }
}

void print_rom_hex(uint8_t *rom, size_t size) {
    printf(".code\n");
    for (size_t i = 0; i + 1 < size; i += 2) {
        uint16_t opcode = (rom[i] << 8) | rom[i + 1];
        printf("%ld - 0x%04X\n", (i / 2) + 2  ,opcode);
    }
}


int main() {
    char nombre[50];
    printf("Ingrese el nombre del juego: ");
    //scanf("%s", nombre);

     
    struct Game game = {
        .window = NULL,
        .renderer = NULL,
    };

    size_t rom_size;
    uint8_t* rom = load_rom("./IBM Logo.ch8", &rom_size);
    if (rom == NULL) {
        fprintf(stderr, "Error trying to open %s file", nombre);
        return 1;
    }

    cpu_registers_t* cpu = createChip8();
    print_rom_hex(rom, rom_size);
    loadProgram(cpu, rom, rom_size);
    
    if (sdl_initialize(&game)) {
        cleanAll(cpu, &game, EXIT_FAILURE);
    }

    

    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                cleanAll(cpu, &game, EXIT_SUCCESS);
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_ESCAPE:
                    cleanAll(cpu, &game, EXIT_SUCCESS);
                    break;
                default:
                    break;
                }
            default:
                break;
            }
        }

        /* Clear screen to black then render the CHIP-8 framebuffer */
        SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
        SDL_RenderClear(game.renderer);

        renderChip8Screen(&game, cpu);

        SDL_RenderPresent(game.renderer);

        SDL_Delay(16);
    }

    cleanAll(cpu, &game, EXIT_SUCCESS);
    return 0;
}

void cleanAll(cpu_registers_t* cpu, struct Game *game, int exit_status) {
    free(cpu);
    game_cleanup(game, exit_status);
}


void game_cleanup(struct Game *game, int exit_status) {
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
    exit(exit_status);
}

bool sdl_initialize(struct Game *game) {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return true;
    }

    game->window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                    SCREEN_HEIGHT, 0);
    if (!game->window) {
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        return true;
    }

    game->renderer = SDL_CreateRenderer(game->window, -1, 0);
    if (!game->renderer) {
        fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
        return true;
    }

    return false;
}
