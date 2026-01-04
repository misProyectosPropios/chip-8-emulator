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
    TEST_ASSERT_MESSAGE( cpu_registers.data_register[0] == 0xFF , "Instruction 6XNN doesn't change the value of the register");
}

void tests2_execute7XNNAddValueNNToRegisterCorrectly(void) {
    cpu_registers_t cpu_registers;
    executeInstruction(0x6001, &cpu_registers);
    executeInstruction(0x7001, &cpu_registers);
    TEST_ASSERT_MESSAGE( cpu_registers.data_register[0] == 0x02 , "Instruction 7XNN doesn't add the value to the register");
}

void tests3_execute8XY0MovesValueFromYToX(void) {
    cpu_registers_t cpu_registers;
    executeInstruction(0x6001, &cpu_registers);
    executeInstruction(0x8100, &cpu_registers);
    TEST_ASSERT_MESSAGE( cpu_registers.data_register[1] == 0x01 , "Instruction 8XY0 doesn't move the value of the register_from to the register_to");
}

void tests4_execute8XY1StoresInXValueOfORBetweenXY(void) {
    cpu_registers_t cpu_registers;
    executeInstruction(0x60F0, &cpu_registers);
    executeInstruction(0x610F, &cpu_registers);
    executeInstruction(0x8101, &cpu_registers);
    TEST_ASSERT_MESSAGE( cpu_registers.data_register[1] == 0xFF , "Instruction 8XY1 doesn't store in X the value of the operation X | Y");
}

void tests5_execute8XY2StoresInXValueOfANDBetweenXY(void) {
    cpu_registers_t cpu_registers;
    executeInstruction(0x60F0, &cpu_registers);
    executeInstruction(0x610F, &cpu_registers);
    executeInstruction(0x8102, &cpu_registers);
    TEST_ASSERT_MESSAGE( cpu_registers.data_register[1] == 0x00 , "Instruction 8XY2 doesn't store in X the value of the operation X & Y");
}

void tests6_execute8XY3StoresInXValueOfXORBetweenXY(void) {
    cpu_registers_t cpu_registers;
    executeInstruction(0x60FC, &cpu_registers);
    executeInstruction(0x61F3, &cpu_registers);
    executeInstruction(0x8103, &cpu_registers);
    TEST_ASSERT_MESSAGE( cpu_registers.data_register[1] == 0x0F , "Instruction 8XY3 doesn't store in X the value of the operation X ^ Y");
}

void tests7_execute8XY4AddsTheValue(void) {
    cpu_registers_t cpu_registers;
    executeInstruction(0x6001, &cpu_registers);
    executeInstruction(0x6102, &cpu_registers);

    executeInstruction(0x63FF, &cpu_registers);
    executeInstruction(0x64FF, &cpu_registers);
    
    executeInstruction(0x6FFF, &cpu_registers);
    executeInstruction(0x8104, &cpu_registers);
    
    TEST_ASSERT_MESSAGE( cpu_registers.data_register[1] == 0x03 , "Instruction 8XY4 doesn't store in X the value of the operation X ^ Y");
    TEST_ASSERT_MESSAGE( cpu_registers.data_register[15] == 0x00, "Instruction 8XY4 doesn't store in VF whether there was carry or not");

    executeInstruction(0x8344, &cpu_registers);
    TEST_ASSERT_MESSAGE( cpu_registers.data_register[15] == 0x01, "Instruction 8XY4 doesn't store in VF whether there was carry or not");    
}

void tests8_execute8XY5SubstractsTheValueAndAddValuesCorrectly(void) {
    cpu_registers_t cpu_registers;
    executeInstruction(0x6001, &cpu_registers); // V0 = 1
    executeInstruction(0x6102, &cpu_registers); // V1 = 2
    executeInstruction(0x8015, &cpu_registers); // V0 = V0 - V1
    TEST_ASSERT_MESSAGE( cpu_registers.data_register[15] == 0x01, "Instruction 8XY5 doesn't store in VF whether there was carry or not");    
    TEST_ASSERT_MESSAGE( cpu_registers.data_register[0] == 0xFF, "Instruction 8X5t doesn't store in VX the result of the operation");   
    
    executeInstruction(0x6005, &cpu_registers); // V0 = 5
    executeInstruction(0x6103, &cpu_registers); // V1 = 3
    executeInstruction(0x8015, &cpu_registers); // V0 = V0 - V1

    TEST_ASSERT_MESSAGE( cpu_registers.data_register[15] == 0x00, "Instruction 8XY5 doesn't store in VF whether there was carry or not");    
    TEST_ASSERT_MESSAGE( cpu_registers.data_register[0] == 0x02, "Instruction 8X55 doesn't store in VX the result of the operation");   
}
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(tests1_execute6XNNBehavesCorrectly);
    RUN_TEST(tests2_execute7XNNAddValueNNToRegisterCorrectly);
    RUN_TEST(tests3_execute8XY0MovesValueFromYToX);
    RUN_TEST(tests4_execute8XY1StoresInXValueOfORBetweenXY);
    RUN_TEST(tests5_execute8XY2StoresInXValueOfANDBetweenXY);
    RUN_TEST(tests6_execute8XY3StoresInXValueOfXORBetweenXY);
    RUN_TEST(tests7_execute8XY4AddsTheValue);
    RUN_TEST(tests8_execute8XY5SubstractsTheValueAndAddValuesCorrectly);
    return UNITY_END();
}