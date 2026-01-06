#include <stdint.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define MEMORY_SIZE 4096

typedef struct cpu_registers {
    uint8_t data_register[16]; //16 different register: from V0 to VF
    uint16_t address_register; //in instruction set its called I
    uint16_t pc;
    uint8_t  display[SCREEN_HEIGHT][SCREEN_WIDTH];
    uint8_t  memory[MEMORY_SIZE];
    uint8_t  sp;
    uint16_t stack[16];
    uint8_t  keypad[16];
    uint8_t  delay_timer;
    uint8_t  sound_timer;
} cpu_registers_t;



// Functionstest/unity

cpu_registers_t* createChip8();

void executeInstruction(uint16_t opcode, cpu_registers_t* cpu_registers);

// Extra functions

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


// Completely auxilary function

int random(int min, int max);