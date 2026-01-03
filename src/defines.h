#include <stdint.h>

typedef struct cpu_registers {
    uint8_t data_register[16];
    uint16_t address_egister;
} cpu_registers_t;

