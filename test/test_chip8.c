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
    TEST_ASSERT_MESSAGE( cpu_registers.data_register[0] == 0x02 , "Instruction 7XNN doens't add the value to the register");
}

void tests3_execute8XY0MovesValueFromYToX(void) {
    cpu_registers_t cpu_registers;
    executeInstruction(0x6001, &cpu_registers);
    executeInstruction(0x8100, &cpu_registers);
    TEST_ASSERT_MESSAGE( cpu_registers.data_register[1] == 0x01 , "Instruction 8XY0 doens't move the value of the register_from to the register_to");
}

void tests4_execute8XY0MovesValueFromYToX(void) {
    cpu_registers_t cpu_registers;
    executeInstruction(0x60F0, &cpu_registers);
    executeInstruction(0x610F, &cpu_registers);
    executeInstruction(0x8101, &cpu_registers);
    TEST_ASSERT_MESSAGE( cpu_registers.data_register[1] == 0xFF , "Instruction 8XY0 doens't change the value of the register");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(tests1_execute6XNNBehavesCorrectly);
    RUN_TEST(tests2_execute7XNNAddValueNNToRegisterCorrectly);
    RUN_TEST(tests3_execute8XY0MovesValueFromYToX);
    return UNITY_END();
}