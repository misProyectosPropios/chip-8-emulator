#include "defines.h"

static FILE *log_file = NULL;


int main() {
    log_file = fopen("./chip8.log", "w");
    if (!log_file) {
        perror("fopen log");
        return 1;
    }
    printf("Creation of the chip8.log file");

    char nombre[50];
    printf("Ingrese el nombre del juego: ");
    scanf("%s", nombre);

     
    struct Game game = {
        .window = NULL,
        .renderer = NULL,
    };

    size_t rom_size;
    uint8_t* rom = load_rom(nombre, &rom_size);
    if (rom == NULL) {
        fprintf(stderr, "Error trying to open %s file", nombre);
        return 1;
    }

    cpu_registers_t* cpu = createChip8();
    print_rom_hex(rom, rom_size);
    loadProgram(cpu, rom, rom_size);
    
/*
    for(int i = 0; i < 100; i++) {
        uint16_t opcode = fetchInstruction(cpu);
        fprintf(log_file, "PC=%03X OPCODE=%04X\n", cpu->pc, opcode);
        executeInstruction(opcode, cpu);
    }
*/
    if (sdl_initialize(&game)) {
        cleanAll(cpu, &game, EXIT_FAILURE);
        return 1;
    }

    

    while (true) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                cleanAll(cpu, &game, EXIT_SUCCESS);
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_1:
                        storeKeyup(cpu, 0x1);
                        break;
                    case SDL_SCANCODE_2:
                        storeKeyup(cpu, 0x2);
                        break;
                    case SDL_SCANCODE_3:
                        storeKeyup(cpu, 0x3);
                        break;
                    case SDL_SCANCODE_4:
                        storeKeyup(cpu, 0xC);
                        break;
                    case SDL_SCANCODE_Q:
                        storeKeyup(cpu, 0x4);
                        break;
                    case SDL_SCANCODE_W:
                        storeKeyup(cpu, 0x5);
                        break;
                    case SDL_SCANCODE_E:
                        storeKeyup(cpu, 0x6);
                        break;
                    case SDL_SCANCODE_R:
                        storeKeyup(cpu, 0xD);
                        break;
                    case SDL_SCANCODE_A:
                        storeKeyup(cpu, 0x7);
                        break;
                    case SDL_SCANCODE_S:
                        storeKeyup(cpu, 0x8);
                        break;
                    case SDL_SCANCODE_D:
                        storeKeyup(cpu, 0x9);
                        break;
                    case SDL_SCANCODE_F:
                        storeKeyup(cpu, 0xE);
                        break;
                    case SDL_SCANCODE_Z:
                        storeKeyup(cpu, 0xA);
                        break;
                    case SDL_SCANCODE_X:
                        storeKeyup(cpu, 0x0);
                        break;
                    case SDL_SCANCODE_C:
                        storeKeyup(cpu, 0xB);
                        break;
                    case SDL_SCANCODE_V:
                        storeKeyup(cpu, 0xF);
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_ESCAPE:
                        cleanAll(cpu, &game, EXIT_SUCCESS);
                        break;
                    case SDL_SCANCODE_1:
                        storeKeydown(cpu, 0x1);
                        break;
                    case SDL_SCANCODE_2:
                        storeKeydown(cpu, 0x2);
                        break;
                    case SDL_SCANCODE_3:
                        storeKeydown(cpu, 0x3);
                        break;
                    case SDL_SCANCODE_4:
                        storeKeydown(cpu, 0xC);
                        break;
                    case SDL_SCANCODE_Q:
                        storeKeydown(cpu, 0x4);
                        break;
                    case SDL_SCANCODE_W:
                        storeKeydown(cpu, 0x5);
                        break;
                    case SDL_SCANCODE_E:
                        storeKeydown(cpu, 0x6);
                        break;
                    case SDL_SCANCODE_R:
                        storeKeydown(cpu, 0xD);
                        break;
                    case SDL_SCANCODE_A:
                        storeKeydown(cpu, 0x7);
                        break;
                    case SDL_SCANCODE_S:
                        storeKeydown(cpu, 0x8);
                        break;
                    case SDL_SCANCODE_D:
                        storeKeydown(cpu, 0x9);
                        break;
                    case SDL_SCANCODE_F:
                        storeKeydown(cpu, 0xE);
                        break;
                    case SDL_SCANCODE_Z:
                        storeKeydown(cpu, 0xA);
                        break;
                    case SDL_SCANCODE_X:
                        storeKeydown(cpu, 0x0);
                        break;
                    case SDL_SCANCODE_C:
                        storeKeydown(cpu, 0xB);
                        break;
                    case SDL_SCANCODE_V:
                        storeKeydown(cpu, 0xF);
                        break;
                    default:
                        break;
                }
                default:
                    break;
            }

            
        }
        int16_t opcode = fetchInstruction(cpu);
        log_keypad(log_file, cpu);
        log_timers(log_file, cpu);
        fprintf(log_file, "PC=%03X OPCODE=%04X\n", cpu->pc, (uint16_t)opcode);
        
        executeInstruction(opcode, cpu);
        decrementTimer(cpu);
        if(cpu->screen_modify) {
            SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
            SDL_RenderClear(game.renderer);
            renderChip8Screen(&game, cpu);
            SDL_RenderPresent(game.renderer);
            cpu->screen_modify = 0;
        }

        SDL_Delay(4);
    }

    //cleanAll(cpu, &game, EXIT_SUCCESS);
    return 0;
}


static void renderChip8Screen(struct Game *game, cpu_registers_t *cpu) {
    int scale = (SCREEN_WIDTH) / (CHIP8_SCREEN_WIDTH);
    
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

void print_rom_hex(uint8_t *rom, size_t size) {
    printf(".code\n");
    for (size_t i = 0; i + 1 < size; i += 2) {
        uint16_t opcode = (rom[i] << 8) | rom[i + 1];
        printf("%ld - 0x%04X\n", (i / 2) + 2  ,opcode);
    }
}


void log_keypad(FILE *log_file, cpu_registers_t *cpu) {
    fprintf(log_file, "KEYPAD=");
    for (int i = 0; i < 16; i++) {
        fprintf(log_file, "%X:%d ", i, cpu->keypad[i]);
    }
}

void log_timers(FILE *log_file, cpu_registers_t *cpu) {
    fprintf(log_file, "DELAY_TIMER=%d", cpu->delay_timer);
    fprintf(log_file, "SOUND_TIMER=%d", cpu->sound_timer);
}


void cleanAll(cpu_registers_t* cpu, struct Game *game, int exit_status) {
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
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
