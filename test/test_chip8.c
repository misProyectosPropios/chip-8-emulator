#include "unity.h"
#include "../src/chip8.h"

static cpu_registers_t cpu;

void setUp(void) {
    cpu_reset(&cpu);
}

void tearDown(void) {
    // nothing for now
}

void test_OP_6XNN_sets_Vx_to_NN(void) {
    // Precondition
    TEST_ASSERT_EQUAL_UINT8(0, cpu.data_register[0]);

    // Execute opcode 0x60AB -> set V0 = 0xAB
    cpu_execute(&cpu, 0x60AB);

    // Verify
    TEST_ASSERT_EQUAL_UINT8(0xAB, cpu.data_register[0]);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_OP_6XNN_sets_Vx_to_NN);
    return UNITY_END();
}