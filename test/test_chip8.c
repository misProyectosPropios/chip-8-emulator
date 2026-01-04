#include "unity.h"
#include "../src/chip8.h"


void setUp(void) {
    
}

void tearDown(void) {
    // nothing for now
}

void tests1_execute6XNNBehavesCorrectly(void) {
    cpu_registers_t cpu_registers;
    executeInstruction(0x60FF, &cpu_registers);
    TEST_ASSERT_MESSAGE( cpu_registers.data_register[0] == 0xFF , "Instruction 6XNN doens't change the value of the register");
}

void tests2_execute7XNNAddValueNNToRegisterCorrectly(void) {
    cpu_registers_t cpu_registers;
    executeInstruction(0x6001, &cpu_registers);
    executeInstruction(0x7001, &cpu_registers);
    TEST_ASSERT_MESSAGE( cpu_registers.data_register[0] == 0x02 , "Instruction 6XNN doens't change the value of the register");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(tests1_execute6XNNBehavesCorrectly);
    RUN_TEST(tests2_execute7XNNAddValueNNToRegisterCorrectly);
    return UNITY_END();
}