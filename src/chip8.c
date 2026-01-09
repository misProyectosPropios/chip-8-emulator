
#include "chip8.h"

cpu_registers_t* createChip8() {
    cpu_registers_t* chip8 = malloc(sizeof(cpu_registers_t));
    if (!chip8) return NULL;

    /* Zero all CPU state first */
    memset(chip8, 0, sizeof(cpu_registers_t));

    /* Initialize registers */
    chip8->pc = PROGRAM_START_ADDRESS; /* Programs start at 0x200 */
    chip8->sp = 0;
    chip8->address_register = 0;
    chip8->delay_timer = 0;
    chip8->sound_timer = 0;
    chip8->screen_modify = 0;

    /* Standard CHIP-8 fontset (0-F), each character 5 bytes */
    static const uint8_t chip8_fontset[80] = {
        0xF0,0x90,0x90,0x90,0xF0, /* 0 */
        0x20,0x60,0x20,0x20,0x70, /* 1 */
        0xF0,0x10,0xF0,0x80,0xF0, /* 2 */
        0xF0,0x10,0xF0,0x10,0xF0, /* 3 */
        0x90,0x90,0xF0,0x10,0x10, /* 4 */
        0xF0,0x80,0xF0,0x10,0xF0, /* 5 */
        0xF0,0x80,0xF0,0x90,0xF0, /* 6 */
        0xF0,0x10,0x20,0x40,0x40, /* 7 */
        0xF0,0x90,0xF0,0x90,0xF0, /* 8 */
        0xF0,0x90,0xF0,0x10,0xF0, /* 9 */
        0xF0,0x90,0xF0,0x90,0x90, /* A */
        0xE0,0x90,0xE0,0x90,0xE0, /* B */
        0xF0,0x80,0x80,0x80,0xF0, /* C */
        0xE0,0x90,0x90,0x90,0xE0, /* D */
        0xF0,0x80,0xF0,0x80,0xF0, /* E */
        0xF0,0x80,0xF0,0x80,0x80  /* F */
    };

    /* Load fontset into memory at 0x50 (conventional location) */
    memcpy(&chip8->memory[0x50], chip8_fontset, sizeof(chip8_fontset));

    return chip8;
}

void loadProgram(cpu_registers_t *cpu, uint8_t *program, size_t size) {
    for(size_t i = 0; i < size; i++) {
        cpu->memory[i + 0x200] = program[i];
    }
}


uint16_t fetchInstruction(cpu_registers_t* cpu) {
    uint16_t opcode =
    ((uint16_t)cpu->memory[cpu->pc] << 8) |
    (uint16_t)cpu->memory[cpu->pc + 1];
    return opcode;
}

void decrementTimer(cpu_registers_t* cpu) {
    cpu->delay_timer = (cpu->delay_timer == 0) ? 0 : cpu->delay_timer - 1;
    cpu->sound_timer = (cpu->sound_timer == 0) ? 0 : cpu->sound_timer - 1;
}

void executeInstruction(uint16_t opcode, cpu_registers_t* cpu_registers) {

    cpu_registers->pc += 2;
    uint8_t X = (opcode & 0x0F00) >> 8;
    uint8_t VX = cpu_registers->data_register[X];
    uint8_t NN = opcode & 0x00FF;
    uint16_t NNN = opcode & 0x0FFF;
    uint8_t Y = (opcode & 0x00F0) >> 4;
    uint8_t VY = cpu_registers->data_register[Y];

    switch((opcode & 0xF000) >> 12) {
        case 0x0:
            if ((opcode & 0xFFFF) == 0x00E0) {
                cpu_registers->screen_modify = 1;
                memset(cpu_registers->display, 0, CHIP8_SCREEN_WIDTH * CHIP8_SCREEN_HEIGHT * sizeof(uint8_t));
            } 
            else if ((opcode & 0xFFFF) == 0x00EE) {
                cpu_registers->pc = cpu_registers->stack[--cpu_registers->sp];
            }
            else if ((opcode & 0xF000) == 0x0000) {
                //Its not neccesary to implement
            }
            break;
        case 0x1:
            cpu_registers->pc = opcode & 0x0FFF;
            break;
        case 0x2:
            cpu_registers->stack[cpu_registers->sp++] = cpu_registers->pc;
            cpu_registers->pc = opcode & 0x0FFF;
            break;
        case 0x3:
    
            if (VX == NN) {
                cpu_registers->pc += 2;
            }
            break;
        case 0x4:
            //Skip the following instruction if the value of register VX equals NN

            if (VX != NN) {
                cpu_registers->pc += 2;
            }
            break;
        case 0x5:
            //Skip the following instruction if the value of register VX is equal to the value of register VY 
            
            if (VX == VY) {
                cpu_registers->pc += 2;
            }
            break;
        case 0x6:
            setValueRegister(X, NN, cpu_registers);
            break;
        case 0x7:
            addValueToRegister(X, NN, cpu_registers);
            break;
        case 0x8:
            if ((opcode & 0xF00F) == 0x8000) {
                moveValueFromXToY(Y, X, cpu_registers);
            }
            else if ((opcode & 0xF00F) == 0x8001) {
                or(Y, X, cpu_registers);
            }
            else if ((opcode & 0xF00F) == 0x8002) {
                and(Y, X, cpu_registers);
            }
            else if ((opcode & 0xF00F) == 0x8003) {
                
                xor(Y, X, cpu_registers);
            }
            else if ((opcode & 0xF00F) == 0x8004) {
                sum(Y, X, cpu_registers);
            }
            else if ((opcode & 0xF00F) == 0x8005) {
                substract(X, Y, cpu_registers);
            }
            else if ((opcode & 0xF00F) == 0x8006) {
                shr(Y, X, cpu_registers);
            }
            else if ((opcode & 0xF00F) == 0x8007) {
                substractInverse(Y, X, cpu_registers);
            }
            else if ((opcode & 0xF00F) == 0x800E) {
                shl(Y, X, cpu_registers);
            }
            break;
        case 0x9:
            if (VX != VY) {
                cpu_registers->pc += 2;
            }
                break;
        case 0xA:
            //Store memory address NNN in register I
            cpu_registers->address_register = NNN;
            break;
        case 0xB:
            //Jump to address NNN + V0
            cpu_registers->pc = NNN + cpu_registers->data_register[0];
            break;
        case 0xC:
            //Set VX to a random number with a mask of NN
            uint8_t randomValue = randomBetween(0, 0xFF);
            cpu_registers->data_register[X] = NN & randomValue;
            break;
        case 0xD:
            /* Draw a sprite at position VX, VY with N bytes of sprite data starting at I.
           Set VF to 1 if any set pixels are changed to unset (collision), otherwise 0. */
            cpu_registers->data_register[0xF] = 0;
            cpu_registers->screen_modify = 1;
            
            uint16_t I = cpu_registers->address_register;
            uint8_t N = opcode & 0x000F;
            for (int row = 0; row < N; row++) {
                uint8_t spriteByte = cpu_registers->memory[I + row];
                for (int bit = 0; bit < 8; bit++) {
                    uint8_t spritePixel = ((spriteByte >> (7 - bit)) & 0x1) ? 0xFF : 0x00;

                    if (!spritePixel) continue;
                    uint16_t x = (VX + bit) % CHIP8_SCREEN_WIDTH;
                    uint16_t y = (VY + row) % CHIP8_SCREEN_HEIGHT;
                    uint8_t *screenPixel = &cpu_registers->display[y][x];
                    if (*screenPixel != 0 && spritePixel != 0) {
                        cpu_registers->data_register[0xF] = 1;
                    }
                    *screenPixel = (*screenPixel) ^ spritePixel;
                }
            }
            break;
        case 0xE:
            if ((opcode & 0xF0FF) == 0xE09E) {
                /*
                Skip the following instruction if the key corresponding to the hex value currently stored in register VX is pressed
                */

               uint8_t X = (opcode & 0x0F00) >> 8;
               uint8_t key = cpu_registers->data_register[X];
               if (key < 16 && cpu_registers->keypad[key]) {
                    cpu_registers->pc += 2;
               }
            }
            else if ((opcode & 0xF0FF) == 0xE0A1) {
                //Skip the following instruction if the key corresponding to the hex value currently stored in register VX is not pressed
                uint8_t X = (opcode & 0x0F00) >> 8;
               uint8_t key = cpu_registers->data_register[X];
               if (!cpu_registers->keypad[key]) {
                    cpu_registers->pc += 2;
               }
            }
            break;
        case 0xF:
            if ((opcode & 0xF0FF) == 0xF007) {
                //Store the current value of the delay timer in register VX
                uint8_t X = (opcode & 0x0F00) >> 8;
                cpu_registers->data_register[X] = cpu_registers->delay_timer;
            }
            else if ((opcode & 0xF0FF) == 0xF00A) {
                //Do something
                uint8_t X = (opcode & 0x0F00) >> 8;
                if (isThereNotAnyKeyPress(X, cpu_registers)) {
                    cpu_registers->pc = cpu_registers->pc - 2;
                }
            }
            else if ((opcode & 0xF0FF) == 0xF015) {
                //Set the delay timer to the value of register VX
                uint8_t X = (opcode & 0x0F00) >> 8;
                cpu_registers->delay_timer = cpu_registers->data_register[X];
            }
            else if ((opcode & 0xF0FF) == 0xF018) {
                //Set the sound timer to the value of register VX
                uint8_t X = (opcode & 0x0F00) >> 8;
                cpu_registers->sound_timer = cpu_registers->data_register[X];
            }
            else if ((opcode & 0xF0FF) == 0xF01E) {
                //Add the value stored in register VX to register I
                uint8_t X = (opcode & 0x0F00) >> 8;
                uint8_t Vx = cpu_registers->data_register[X];
                cpu_registers->address_register += Vx;
            }
            else if ((opcode & 0xF0FF) == 0xF029) {
                //Do something
                uint8_t Vx = (opcode & 0x0F00) >> 8;
                uint8_t digit = cpu_registers->data_register[Vx];
                cpu_registers->address_register = FONTSET_START_ADDRESS + 5 * digit;
            }
            else if ((opcode & 0xF0FF) == 0xF033) {
                //Store the binary-coded decimal equivalent of the value stored in register VX at addresses I, I + 1, and I + 2
                uint8_t x = (opcode & 0x0F00) >> 8;
                storeBinaryCodedDecimal(x, cpu_registers);
            }
            else if ((opcode & 0xF0FF) == 0xF055) {
                //Store the values of registers V0 to VX inclusive in memory starting at address I. I is set to I + X + 1 after operation
                uint8_t X = (opcode & 0x0F00) >> 8;
                for(int i = 0; i <= X; i++) {
                    cpu_registers->memory[cpu_registers->address_register] = cpu_registers->data_register[i];
                    cpu_registers->address_register++;
                }
            }
            else if ((opcode & 0xF0FF) == 0xF065) {
                //Fill registers V0 to VX inclusive with the values stored in memory starting at address I. I is set to I + X + 1 after operation
                uint8_t X = (opcode & 0x0F00) >> 8;
                for(int i = 0; i <= X; i++) {
                    cpu_registers->data_register[i] = cpu_registers->memory[cpu_registers->address_register];
                    cpu_registers->address_register++;
                }
            }
            break;
    }
}

void setValueRegister(uint8_t cpu_register, uint8_t valueToStore, cpu_registers_t* cpu_registers) {
     cpu_registers->data_register[cpu_register] = valueToStore;
}

void addValueToRegister(uint8_t cpu_register, uint8_t valueToAdd, cpu_registers_t* cpu_registers) {
    cpu_registers->data_register[cpu_register] = cpu_registers->data_register[cpu_register] + valueToAdd;
}

void moveValueFromXToY(uint8_t register_from, uint8_t register_to, cpu_registers_t* cpu_registers) {
    cpu_registers->data_register[register_to] = cpu_registers->data_register[register_from];
}

void or(uint8_t register_from, uint8_t register_to, cpu_registers_t* cpu_registers) {
    cpu_registers->data_register[register_to] = cpu_registers->data_register[register_from] | cpu_registers->data_register[register_to];
    cpu_registers->data_register[0xF] = 0;
}

void and(uint8_t register_from, uint8_t register_to, cpu_registers_t* cpu_registers) {
    cpu_registers->data_register[register_to] = cpu_registers->data_register[register_from] & cpu_registers->data_register[register_to];
    cpu_registers->data_register[0xF] = 0;
}

void xor(uint8_t register_from, uint8_t register_to, cpu_registers_t* cpu_registers) {
    cpu_registers->data_register[register_to] = cpu_registers->data_register[register_from] ^ cpu_registers->data_register[register_to];
    cpu_registers->data_register[0xF] = 0;
}

void sum(uint8_t register_from, uint8_t register_to, cpu_registers_t* cpu_registers) {
    uint8_t fromValue = cpu_registers->data_register[register_from];
    uint8_t toValue = cpu_registers->data_register[register_to];
    uint16_t sum = fromValue + toValue;
    cpu_registers->data_register[register_to] = sum & 0xFF;
    cpu_registers->data_register[0xF] = (sum > 0xFF) ? 1 : 0;
}

void substract(uint8_t X, uint8_t Y, cpu_registers_t* cpu) {
    uint8_t VX = cpu->data_register[X];
    uint8_t VY = cpu->data_register[Y];
    uint8_t substract = VX - VY;
    cpu->data_register[X] = substract & 0xFF;
    cpu->data_register[0xF] = (VY <= VX) ? 1 : 0;
}

void shr(uint8_t X, uint8_t Y, cpu_registers_t* cpu) {
    Y++;
    uint8_t value = cpu->data_register[X];
    cpu->data_register[X] = value >> 1;
    cpu->data_register[0xF] = value & 0x01;
}

void substractInverse(uint8_t Y, uint8_t X, cpu_registers_t* cpu_registers) {
    uint8_t VY = cpu_registers->data_register[Y];
    uint8_t VX = cpu_registers->data_register[X];
    uint8_t substract = VY - VX;
    cpu_registers->data_register[X] = substract;
    cpu_registers->data_register[0xF] = (VY >= VX) ? 1 : 0;
}

void shl(uint8_t X, uint8_t Y, cpu_registers_t* cpu) {
    Y++;
    uint8_t value = cpu->data_register[X];
    cpu->data_register[X] = value << 1;
    cpu->data_register[0xF] = (value & 0x80) >> 7;
}

void storeBinaryCodedDecimal(uint8_t X, cpu_registers_t* cpu_registers) {
    uint8_t Vx = cpu_registers->data_register[X];
    uint8_t valueToStore; 
    valueToStore =  Vx / 100;
    cpu_registers->memory[cpu_registers->address_register] = valueToStore;
    Vx = Vx % 100;
    valueToStore =  Vx / 10;
    cpu_registers->memory[cpu_registers->address_register + 1] = valueToStore;
    Vx = Vx % 10;
    valueToStore =  Vx;
    cpu_registers->memory[cpu_registers->address_register + 2] = valueToStore;

}

uint8_t randomBetween(int min, int max) {
   return rand() % (max - min + 1) + min;
}

uint8_t isThereNotAnyKeyPress(uint8_t X, cpu_registers_t* cpu_registers) {
    for(int i = 0; i < 16; i++) {
        if (cpu_registers->keypad[i]) {
            cpu_registers->data_register[X] = cpu_registers->keypad[i];
            return 0;
        }
    }
    return 1;
}