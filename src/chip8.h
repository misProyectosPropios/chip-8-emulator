#include <stdint.h>

typedef struct cpu_registers {
    uint8_t data_register[16]; //16 different register: from V0 to VF
    uint16_t address_register; //in instruction set its called I
    uint16_t pc;
    uint8_t display[32][64];
    uint8_t  memory[4096];
    uint8_t  keypad[16];
    uint8_t  delay_timer;
    uint8_t  sound_timer;
} cpu_registers_t;


// Functions

void executeInstruction(uint16_t opcode, cpu_registers_t* cpu_registers);

void setValueRegister(uint8_t cpu_register, uint8_t valueToStore, cpu_registers_t* cpu_registers);

void addValueToRegister(uint8_t cpu_register, uint8_t valueToAdd, cpu_registers_t* cpu_registers);

void moveValueFromXToY(uint8_t register_from, uint8_t register_to, cpu_registers_t* cpu_registers);

void storeInXValueOfOrBetweenXY(uint8_t register_from, uint8_t register_to, cpu_registers_t* cpu_registers);

void storeInXValueOfANDBetweenXY(uint8_t register_from, uint8_t register_to, cpu_registers_t* cpu_registers);

void storeInXValueOfXORBetweenXY(uint8_t register_from, uint8_t register_to, cpu_registers_t* cpu_registers);

void storeInXValueOfSumBetweenXYStoringCarry(uint8_t register_from, uint8_t register_to, cpu_registers_t* cpu_registers);

void storeInXValueOfSubstractBetweenXYStoringCarry(uint8_t register_from, uint8_t register_to, cpu_registers_t* cpu_registers);

void storeInXValueOfRightShiftBetweenXYStoringCarry(uint8_t register_from, uint8_t register_to, cpu_registers_t* cpu_registers);

void storeInXValueOfSubstractVXFromVYStoringCarry(uint8_t register_from, uint8_t register_to, cpu_registers_t* cpu_registers);

void storeInXValueOfLeftShiftBetweenXYStoringCarry(uint8_t register_from, uint8_t register_to_write, cpu_registers_t* cpu_registers);