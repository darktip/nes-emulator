#include "bitwise_utils.h"

using namespace nes;

bool nes::isSigned(uint8_t val)
{
    return val & SIGN_BIT_INT8;
}

bool nes::isZero(uint8_t val)
{
    return val == 0;
}

bool nes::testBitMask8Bit(uint8_t bit, uint8_t value)
{
    return (bit & value) > 0;
}

bool nes::testBitMask16Bit(uint16_t bit, uint16_t value)
{
    return (bit & value) > 0;
}

bool nes::isCarryFromUnsignedAdd(uint16_t value)
{
    return value > CARRY_TEST;
}

bool nes::isOverflowFromSignedAdd(uint8_t a, uint8_t b, uint8_t result)
{
    return ((a ^ result) & (b ^ result) & SIGN_BIT_INT8) != 0;
}
