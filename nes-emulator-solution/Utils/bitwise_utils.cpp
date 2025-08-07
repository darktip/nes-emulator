#include "bitwise_utils.h"

bool isSigned(uint8_t val)
{
    return val & SIGN_BIT_INT8;
}

bool testBitMask8Bit(uint8_t bit, uint8_t value)
{
    return (bit & value) > 0;
}

bool testBitMask16Bit(uint16_t bit, uint16_t value)
{
    return (bit & value) > 0;
}

bool isCarryFromUnsignedAdd(uint16_t value)
{
    return value > CARRY_TEST;
}

bool isOverflowFromSignedAdd(uint8_t a, uint8_t b, uint8_t result)
{
    return ((a ^ result) & (b ^ result) & SIGN_BIT_INT8) != 0;
}
