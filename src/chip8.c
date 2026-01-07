#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "chip8.h"

cpu_registers_t* createChip8() {
    cpu_registers_t* chip8 = malloc(sizeof(cpu_registers_t));
    if (!chip8) return NULL;

    /* Zero all CPU state first */
    memset(chip8, 0, sizeof(cpu_registers_t));

    /* Initialize registers */
    chip8->pc = 0x200; /* Programs start at 0x200 */
    chip8->sp = 0;
    chip8->address_register = 0;
    chip8->delay_timer = 0;
    chip8->sound_timer = 0;

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

void executeInstruction(uint16_t opcode, cpu_registers_t* cpu_registers) {

    cpu_registers->pc += 2;

    if ((opcode & 0xFFFF) == 0x00E0) {
        memset(cpu_registers->display, 0, CHIP8_SCREEN_WIDTH * CHIP8_SCREEN_HEIGHT);
    } 
    else if ((opcode & 0xFFFF) == 0x00EE) {
        cpu_registers->pc = cpu_registers->stack[--cpu_registers->sp];
        cpu_registers->pc += 2;
    }
    else if ((opcode & 0xF000) == 0x0000) {
        //Its not neccesary to implement
    }
    else if ((opcode & 0xF000) == 0x1000) {
        cpu_registers->pc += opcode & 0x0FFF;
    }
    else if ((opcode & 0xF000) == 0x2000) {
        cpu_registers->stack[++cpu_registers->sp] = cpu_registers->pc;
        cpu_registers->pc = opcode & 0x0FFF;
    }
    else if ((opcode & 0xF000) == 0x3000) {
        //Skip the following instruction if the value of register VX equals NN
        uint8_t X = (opcode & 0x0F00) >> 8;
        uint8_t NN = opcode & 0x00FF;
        uint8_t regValue = cpu_registers->data_register[X];
        if (regValue == NN) {
            cpu_registers->pc += 2;
        }
    }
    else if ((opcode & 0xF000) == 0x4000) {
        //Skip the following instruction if the value of register VX equals NN
        uint8_t X = (opcode & 0x0F00) >> 8;
        uint8_t NN = opcode & 0x00FF;
        uint8_t regValue = cpu_registers->data_register[X];
        if (regValue != NN) {
            cpu_registers->pc += 2;
        }
    }
    else if ((opcode & 0xF00F) == 0x5000) {
        //Skip the following instruction if the value of register VX is equal to the value of register VY 
        uint8_t X = (opcode & 0x0F00) >> 8;
        uint8_t Y = (opcode & 0x00F0) >> 4;
        uint8_t regVX = cpu_registers->data_register[X];
        uint8_t regVY = cpu_registers->data_register[Y];
        if (regVX == regVY) {
            cpu_registers->pc += 2;
        }
    }
    else if ((opcode & 0xF000) == 0x6000) {
        uint8_t valueToStore = opcode & (0x00FF);
        uint8_t cpu_register = (opcode & (0x0F00)) >> 8;
        setValueRegister(cpu_register, valueToStore, cpu_registers);
    } 
    else if ((opcode & 0xF000) == 0x7000) {
        uint8_t valueToAdd = opcode & (0x00FF);
        uint8_t cpu_register = (opcode & (0x0F00)) >> 8;
        addValueToRegister(cpu_register, valueToAdd, cpu_registers);
    }
    else if ((opcode & 0xF00F) == 0x8000) {
        uint8_t register_to = (opcode & (0x0F00)) >> 8;
        uint8_t register_from = (opcode & (0x00F0)) >> 4;
        moveValueFromXToY(register_from, register_to, cpu_registers);
    }
    else if ((opcode & 0xF00F) == 0x8001) {
        uint8_t register_to = (opcode & (0x0F00)) >> 8;
        uint8_t register_from = (opcode & (0x00F0)) >> 4;
        storeInXValueOfOrBetweenXY(register_from, register_to, cpu_registers);
    }
    else if ((opcode & 0xF00F) == 0x8002) {
        uint8_t register_to = (opcode & (0x0F00)) >> 8;
        uint8_t register_from = (opcode & (0x00F0)) >> 4;
        storeInXValueOfANDBetweenXY(register_from, register_to, cpu_registers);
    }
    else if ((opcode & 0xF00F) == 0x8003) {
        uint8_t register_to = (opcode & (0x0F00)) >> 8;
        uint8_t register_from = (opcode & (0x00F0)) >> 4;
        storeInXValueOfXORBetweenXY(register_from, register_to, cpu_registers);
    }
    else if ((opcode & 0xF00F) == 0x8004) {
        uint8_t register_to = (opcode & (0x0F00)) >> 8;
        uint8_t register_from = (opcode & (0x00F0)) >> 4;
        storeInXValueOfSumBetweenXYStoringCarry(register_from, register_to, cpu_registers);
    }
    else if ((opcode & 0xF00F) == 0x8005) {
        uint8_t register_to = (opcode & (0x0F00)) >> 8;
        uint8_t register_from = (opcode & (0x00F0)) >> 4;
        storeInXValueOfSubstractBetweenXYStoringCarry(register_from, register_to, cpu_registers);
    }
    else if ((opcode & 0xF00F) == 0x8006) {
        uint8_t register_to = (opcode & (0x0F00)) >> 8;
        uint8_t register_from = (opcode & (0x00F0)) >> 4;
        storeInXValueOfRightShiftBetweenXYStoringCarry(register_from, register_to, cpu_registers);
    }
    else if ((opcode & 0xF00F) == 0x8007) {
        uint8_t register_to = (opcode & (0x0F00)) >> 8;
        uint8_t register_from = (opcode & (0x00F0)) >> 4;
        storeInXValueOfSubstractVXFromVYStoringCarry(register_from, register_to, cpu_registers);
    }
    else if ((opcode & 0xF00F) == 0x800E) {
        uint8_t register_to = (opcode & (0x0F00)) >> 8;
        uint8_t register_from = (opcode & (0x00F0)) >> 4;
        storeInXValueOfLeftShiftBetweenXYStoringCarry(register_from, register_to, cpu_registers);
    }
    else if ((opcode & 0xF000) == 0xA000) {
        //Store memory address NNN in register I
        //uint16_t value;
    }
    else if ((opcode & 0xF000) == 0xB000) {
        //Jump to address NNN + V0
        cpu_registers->pc += (opcode & 0x0FFF) + cpu_registers->data_register[0];
    }
    else if ((opcode & 0xF000) == 0xC000) {
        //Set VX to a random number with a mask of NN
        uint8_t X = (opcode & 0x0F00) >> 8;
        uint8_t NN = opcode & 0x0FFF;
        uint8_t randomValue = randomBetween(0, 0xFF);
        cpu_registers->data_register[X] = NN && randomValue;
    }
        else if ((opcode & 0xF000) == 0xD000) {
            /* Draw a sprite at position VX, VY with N bytes of sprite data starting at I.
               Set VF to 1 if any set pixels are changed to unset (collision), otherwise 0. */
            cpu_registers->data_register[0xF] = 0;
            uint16_t I = cpu_registers->address_register;
            uint8_t N = opcode & 0x000F;
            uint8_t VY = cpu_registers->data_register[(opcode & 0x00F0) >> 4];
            uint8_t VX = cpu_registers->data_register[(opcode & 0x0F00) >> 8];

            for (int row = 0; row < N; row++) {
                uint8_t spriteByte = cpu_registers->memory[I + row];
                for (int bit = 0; bit < 8; bit++) {
                    uint8_t spritePixel = (spriteByte >> (7 - bit)) & 0x1;
                    if (!spritePixel) continue;

                    uint16_t x = (VX + bit) % CHIP8_SCREEN_WIDTH;
                    uint16_t y = (VY + row) % CHIP8_SCREEN_HEIGHT;

                    uint8_t *screenPixel = &cpu_registers->display[y][x];
                    if (*screenPixel == 1 && spritePixel == 1) {
                        cpu_registers->data_register[0xF] = 1;
                    }
                    *screenPixel = (*screenPixel) ^ spritePixel;
                }
            }
        }
    else if ((opcode & 0xF0FF) == 0xE09E) {
        /*
        Skip the following instruction if the key corresponding to the hex value currently stored in register VX is pressed
        */
    }
    else if ((opcode & 0xF0FF) == 0xE0A1) {
        //Do something
    }
    else if ((opcode & 0xF0FF) == 0xF007) {
        //Do something
    }
    else if ((opcode & 0xF0FF) == 0xF007) {
        //Do something
    }
    else if ((opcode & 0xF0FF) == 0xF00A) {
        //Do something
    }
    else if ((opcode & 0xF0FF) == 0xF015) {
        //Do something
    }
    else if ((opcode & 0xF0FF) == 0xF018) {
        //Do something
    }
    else if ((opcode & 0xF0FF) == 0xF01E) {
        //Do something
    }
    else if ((opcode & 0xF0FF) == 0xF029) {
        //Do something
    }
    else if ((opcode & 0xF0FF) == 0xF033) {
        //Do something
    }
    else if ((opcode & 0xF0FF) == 0xF055) {
        //Do something
    }
    else if ((opcode & 0xF0FF) == 0xF065) {
        //Do something
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

void storeInXValueOfOrBetweenXY(uint8_t register_from, uint8_t register_to, cpu_registers_t* cpu_registers) {
    cpu_registers->data_register[register_to] = cpu_registers->data_register[register_from] | cpu_registers->data_register[register_to];
}

void storeInXValueOfANDBetweenXY(uint8_t register_from, uint8_t register_to, cpu_registers_t* cpu_registers) {
    cpu_registers->data_register[register_to] = cpu_registers->data_register[register_from] & cpu_registers->data_register[register_to];
}

void storeInXValueOfXORBetweenXY(uint8_t register_from, uint8_t register_to, cpu_registers_t* cpu_registers) {
    cpu_registers->data_register[register_to] = cpu_registers->data_register[register_from] ^ cpu_registers->data_register[register_to];
}

void storeInXValueOfSumBetweenXYStoringCarry(uint8_t register_from, uint8_t register_to, cpu_registers_t* cpu_registers) {
    uint8_t fromValue = cpu_registers->data_register[register_from];
    uint8_t toValue = cpu_registers->data_register[register_to];
    uint16_t sum = fromValue + toValue;
    cpu_registers->data_register[register_to] = sum & 0xFF;
    cpu_registers->data_register[0xF] = (sum > 0xFF) ? 1 : 0;
}

void storeInXValueOfSubstractBetweenXYStoringCarry(uint8_t register_from, uint8_t register_to, cpu_registers_t* cpu_registers) {
    uint8_t fromValue = cpu_registers->data_register[register_from];
    uint8_t toValue = cpu_registers->data_register[register_to];
    uint16_t substract = toValue - fromValue;
    cpu_registers->data_register[register_to] = substract & 0xFF;
    cpu_registers->data_register[0xF] = (toValue < fromValue) ? 1 : 0;
}

void storeInXValueOfRightShiftBetweenXYStoringCarry(uint8_t register_from, uint8_t register_to_write, cpu_registers_t* cpu_registers) {
    uint8_t fromValue = cpu_registers->data_register[register_from];
    uint8_t leastSignificantBit = fromValue & 0x01;
    cpu_registers->data_register[register_to_write] = fromValue >> 1;
    cpu_registers->data_register[0xF] = leastSignificantBit;
}

void storeInXValueOfSubstractVXFromVYStoringCarry(uint8_t register_from, uint8_t register_to, cpu_registers_t* cpu_registers) {
    uint8_t fromValue = cpu_registers->data_register[register_from];
    uint8_t toValue = cpu_registers->data_register[register_to];
    uint16_t substract = fromValue - toValue;
    cpu_registers->data_register[register_to] = substract & 0xFF;
    cpu_registers->data_register[0xF] = (toValue > fromValue) ? 1 : 0;
}

void storeInXValueOfLeftShiftBetweenXYStoringCarry(uint8_t register_from, uint8_t register_to_write, cpu_registers_t* cpu_registers) {
    uint8_t fromValue = cpu_registers->data_register[register_from];
    uint8_t mostSignificantBit = fromValue & 0b10000000;
    cpu_registers->data_register[register_to_write] = fromValue << 1;
    cpu_registers->data_register[0xF] = mostSignificantBit;
}


uint8_t randomBetween(int min, int max) {
   return rand() % (max - min + 1) + min;
}