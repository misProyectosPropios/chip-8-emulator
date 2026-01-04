#include <stdint.h>
#include "chip8.h"


void executeInstruction(uint16_t opcode, cpu_registers_t* cpu_registers) {
    //6XNN 	Store number NN in register VX
    uint8_t valueToStore = opcode && (0x00FF);
    uint8_t cpu_register = (opcode && (0x0F00)) >> 8;
    cpu_registers->data_register[cpu_register] = valueToStore;
}