#include <stdint.h>
#include "chip8.h"

cpu_registers_t* createChip8() {
    cpu_registers_t* chip8 = 0;
    return chip8;
}

void executeInstruction(uint16_t opcode, cpu_registers_t* cpu_registers) {

    cpu_registers->pc += 2;

    if ((opcode & 0xFFFF) == 0x00E0) {
        memset(cpu_registers->display, 0, SCREEN_WIDTH * SCREEN_HEIGHT);
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

    }
    else if ((opcode & 0xF000) == 0x4000) {

    }
    else if ((opcode & 0xF00F) == 0x5000) {

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
    }
    else if ((opcode & 0xF000) == 0xB000) {
        //Jump to address NNN + V0
        cpu_registers->pc += (opcode & 0x0FFF) + cpu_registers->data_register[0];
    }
    else if ((opcode & 0xF000) == 0xC000) {
        //Set VX to a random number with a mask of NN
    }
    else if ((opcode & 0xF000) == 0xD000) {
        /*Draw a sprite at position VX, VY with N bytes of sprite data starting at the address stored in I
Set VF to 01 if any set pixels are changed to unset, and 00 otherwise*/
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


int random(int min, int max){
   return rand() % (max - min + 1) + min;
}