#include "unity.h"
#include "../src/chip8.h"


void setUp(void) {
    
}

void tearDown(void) {
    // nothing for now
}

void firstTests_execute6XNNBehavesCorrectly(void) {
    cpu_registers_t cpu_registers;
    executeInstruction(0x60FF, &cpu_registers);
    TEST_ASSERT_MESSAGE( cpu_registers.data_register[0] == 0xFF , "Instruction 6XNN doens't change the value of the register");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(firstTests_execute6XNNBehavesCorrectly);
    return UNITY_END();
}