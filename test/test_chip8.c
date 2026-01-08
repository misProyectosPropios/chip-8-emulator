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

void tests9_execute8XY6ShiftRigthTheValue(void) {
    cpu_registers_t cpu_registers;
    executeInstruction(0x6103, &cpu_registers); // V1 = 0x0x03
    // Instruction: 8016 (VX = V0, VY = V1)
    executeInstruction(0x8016, &cpu_registers);

    TEST_ASSERT_MESSAGE(cpu_registers.data_register[0] == 0x01, "Instruction 8XY6 doesnt shift the value storing in VY");
    TEST_ASSERT_MESSAGE(cpu_registers.data_register[0xF] == 0x01, "Instruction 8XY6 doesnt store the least significant bit prior to the shift in VF");
    TEST_ASSERT_MESSAGE(cpu_registers.data_register[1] == 0x03, "Instruction 8XY6 doesnt shift the value storing in VY");
}

void tests10_execute8XY7SubstractVYMinusVX(void) {
    cpu_registers_t cpu_registers;
    executeInstruction(0x6001, &cpu_registers); // V0 = 1
    executeInstruction(0x6104, &cpu_registers); // V1 = 4
    executeInstruction(0x8017, &cpu_registers); // V0 = V1 - V0
    TEST_ASSERT_MESSAGE(cpu_registers.data_register[1] == 0x04, "Instruction 8XY7 modify VY while it shouldn't modify it");
    TEST_ASSERT_MESSAGE(cpu_registers.data_register[0] == 0x03, "Instruction 8XY7 doesn't store the substract of both registers");
    TEST_ASSERT_MESSAGE(cpu_registers.data_register[0xF] == 0x00, "Instruction 8XY7 doesnt store the if there was a borrow");

}

void tests11_execute8XYEShiftLeftInstruction(void) {
    cpu_registers_t cpu_registers;
    executeInstruction(0x6102, &cpu_registers); // V1 = 0x0x02
    executeInstruction(0x60Ff, &cpu_registers); // V0 = 0x0xFF
    // Instruction: 801E (VX = V0, VY = V1)
    executeInstruction(0x801E, &cpu_registers);

    TEST_ASSERT_MESSAGE(cpu_registers.data_register[1] == 0x02, "Instruction 8XYE doesnt modify the VY value");
    TEST_ASSERT_MESSAGE(cpu_registers.data_register[0xF] == 0x00, "Instruction 8XY6 doesnt store the least significant bit prior to the shift in VF");
    TEST_ASSERT_MESSAGE(cpu_registers.data_register[0] == 0x04, "Instruction 8XY6 doesnt shift the value storing in VY");
}

void tests12_execute1NNN(void) {
    cpu_registers_t cpu_registers;
    cpu_registers.pc = 0x200;
    executeInstruction(0x1FF0, &cpu_registers); 

    TEST_ASSERT_MESSAGE(cpu_registers.pc == 0xFF0, "Instruction 1NNN doesnt modify the PC value");
    
}

void tests13_execute2NNN(void) {
    cpu_registers_t cpu_registers;
    cpu_registers.pc = 0x200;
    cpu_registers.sp = 0;
    executeInstruction(0x2FF0, &cpu_registers); 

    TEST_ASSERT_MESSAGE(cpu_registers.pc == 0xFF0, "Instruction 2NNN doesnt modify the PC value");
    TEST_ASSERT_MESSAGE(cpu_registers.sp == 1, "Instruction 2NNN doesnt modify the sp value by one");
    TEST_ASSERT_MESSAGE(cpu_registers.stack[0] == 0x202, "Instruction 2NNN doesnt modify the stack correctly");
}

void tests14_execute3NNN_ifTrue(void) {
    cpu_registers_t cpu_registers;
    cpu_registers.pc = 0x200;
    cpu_registers.data_register[0] = 0xF0;
    executeInstruction(0x30F0, &cpu_registers); 

    TEST_ASSERT_MESSAGE(cpu_registers.pc == 0x204, "Instruction 2NNN doesnt skip the following value if true");
}

void tests14_execute3NNN_ifFalse(void) {
    cpu_registers_t cpu_registers;
    cpu_registers.pc = 0x200;
    cpu_registers.data_register[0] = 0x00;
    executeInstruction(0x30F0, &cpu_registers); 

    TEST_ASSERT_MESSAGE(cpu_registers.pc == 0x202, "Instruction 3XYNN doesnt skip the following value if false");
}

void tests15_execute4XNN_ifFalse(void) {
    cpu_registers_t cpu_registers;
    cpu_registers.pc = 0x200;
    cpu_registers.data_register[0] = 0xF0;
    executeInstruction(0x40F0, &cpu_registers); 

    TEST_ASSERT_MESSAGE(cpu_registers.pc == 0x202, "Instruction 4XNN doesnt skip the following value if condition false");
}

void tests15_execute4XNN_ifTrue(void) {
    cpu_registers_t cpu_registers;
    cpu_registers.pc = 0x200;
    cpu_registers.data_register[0] = 0x00;
    executeInstruction(0x40F0, &cpu_registers); 

    TEST_ASSERT_MESSAGE(cpu_registers.pc == 0x204, "Instruction 4XNN doesnt skip the following value if condition true");
}

void tests16_execute5XY0_ifTrue(void) {
    cpu_registers_t cpu_registers;
    cpu_registers.pc = 0x200;
    cpu_registers.data_register[0] = 0x00;
    cpu_registers.data_register[1] = 0x00;
    executeInstruction(0x5010, &cpu_registers); 
    TEST_ASSERT_MESSAGE(cpu_registers.pc == 0x204, "Instruction 5XY0 doesnt skip the following value if condition true");
    
}

void tests16_execute5XY0_ifFalse(void) {
    cpu_registers_t cpu_registers;
    cpu_registers.pc = 0x200;
    cpu_registers.data_register[0] = 0x00;
    cpu_registers.data_register[1] = 0x10;
    executeInstruction(0x5010, &cpu_registers); 
    TEST_ASSERT_MESSAGE(cpu_registers.pc == 0x202, "Instruction 5XY0 skip the following value if condition false");
}

void tests17_execute9XY0_ifTrue(void) {
    cpu_registers_t cpu_registers;
    cpu_registers.pc = 0x200;
    cpu_registers.data_register[0] = 0x00;
    cpu_registers.data_register[1] = 0x10;
    executeInstruction(0x9010, &cpu_registers); 
    TEST_ASSERT_MESSAGE(cpu_registers.pc == 0x204, "Instruction 9XY0 doesnt skip the following value if condition true");
}

void tests17_execute9XY0_ifFalse(void) {
    cpu_registers_t cpu_registers;
    cpu_registers.pc = 0x200;
    cpu_registers.data_register[0] = 0x00;
    cpu_registers.data_register[1] = 0x00;
    executeInstruction(0x9010, &cpu_registers); 
    TEST_ASSERT_MESSAGE(cpu_registers.pc == 0x202, "Instruction 9XY0 skip the following value if condition false");
}

void tests18_executeANNN(void) {
    cpu_registers_t cpu_registers;
    cpu_registers.address_register = 0x00FF;
    executeInstruction(0xA000, &cpu_registers); 
    TEST_ASSERT_MESSAGE(cpu_registers.address_register == 0x0000, "Instruction ANNN modifies the value of address_register");
}

void tests19_executeBNNN(void) {
    cpu_registers_t cpu_registers;
    cpu_registers.pc = 0x0FFF;
    cpu_registers.data_register[0] = 0xFF;
    executeInstruction(0xB00F, &cpu_registers); 
    TEST_ASSERT_MESSAGE(cpu_registers.pc == (0xFF + 0x0F), "Instruction BNNN modifies the value of PC incorrectly");
}

void tests20_executeFX07(void) {
    cpu_registers_t cpu_registers;
    cpu_registers.delay_timer = 0xDF;

    executeInstruction(0xF007, &cpu_registers); 
    TEST_ASSERT_MESSAGE(cpu_registers.data_register[0] == 0xDF, "Instruction FX07 doesnt modify the register selected with the delay timer");
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
    RUN_TEST(tests9_execute8XY6ShiftRigthTheValue);
    RUN_TEST(tests10_execute8XY7SubstractVYMinusVX);
    RUN_TEST(tests11_execute8XYEShiftLeftInstruction);
    RUN_TEST(tests12_execute1NNN);
    RUN_TEST(tests13_execute2NNN);
    RUN_TEST(tests14_execute3NNN_ifTrue);
    RUN_TEST(tests14_execute3NNN_ifFalse);
    RUN_TEST(tests15_execute4XNN_ifTrue);
    RUN_TEST(tests15_execute4XNN_ifFalse);
    RUN_TEST(tests16_execute5XY0_ifFalse);
    RUN_TEST(tests16_execute5XY0_ifTrue);
    RUN_TEST(tests17_execute9XY0_ifTrue);
    RUN_TEST(tests17_execute9XY0_ifFalse);
    RUN_TEST(tests18_executeANNN);
    RUN_TEST(tests19_executeBNNN);
    RUN_TEST(tests20_executeFX07);
    return UNITY_END();
}