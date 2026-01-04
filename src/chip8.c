#include <stdint.h>
#include "chip8.h"


void executeInstruction(uint16_t opcode, cpu_registers_t* cpu_registers) {
    //6XNN 	Store number NN in register VX
    if ((opcode & 0xF000) == 0x6000) {
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


