#include <stdint.h>
#include "chip8.h"


void executeInstruction(uint16_t opcode, cpu_registers_t* cpu_registers) {
    //6XNN 	Store number NN in register VX
    if (opcode & 0x6000) {
        uint8_t valueToStore = opcode & (0x00FF);
        uint8_t cpu_register = (opcode & (0x0F00)) >> 8;
        setValueRegister(cpu_register, valueToStore, cpu_register);
    } 
    if (opcode & 0x7000) {
        uint8_t valueToStore = opcode & (0x00FF);
        uint8_t cpu_register = (opcode & (0x0F00)) >> 8;
        setValueRegister(cpu_register, valueToStore, cpu_register);
    } 
}

setValueRegister(uint8_t cpu_register, uint8_t valueToStore, cpu_registers_t* cpu_registers) {
     cpu_registers->data_register[cpu_register] = valueToStore;
}

addValueToRegister(uint8_t cpu_register, uint8_t valueToAdd, cpu_registers_t* cpu_registers) {
    cpu_registers->data_register[cpu_register] = cpu_registers->data_register[cpu_register] + valueToAdd;
}