#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#define BIT_SET(reg, value) ((reg) |= (1U << (value))) // Set bit value in reg
#define BIT_CLR(reg, value) ((reg) &= ~(1U << (value))) // Clear bit value in reg
#define BIT_TOGGLE(reg, value) ((reg) ^= (1U << (value))) // Toggle bit value in reg
#define BIT_CHECK(reg, value) ((reg) & (1U << (value))) // Check if bit value in reg is set

// Register-level operations (operate on the entire register)
#define REG_SET(reg, value) ((reg) = (value)) // Set the entire register to value
#define REG_CLR(reg) ((reg) = 0) // Clear the entire register
#define REG_TOGGLE(reg) ((reg) ^= 0xFFFFFFFF) // Toggle all bits in the register
#define REG_CHECK(reg, mask) ((reg) & (mask)) // Check if any bits in mask are set in reg

#endif // LIB_H
