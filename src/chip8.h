#include <stdint.h>

typedef struct cpu_registers {
    uint8_t data_register[16]; //16 different register: from V0 to VF
    uint16_t address_register; //in instruction set its called I
} cpu_registers_t;

// Functions

void executeInstruction(uint16_t opcode, cpu_registers_t* cpu_registers);