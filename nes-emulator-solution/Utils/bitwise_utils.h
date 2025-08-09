#pragma once

#include <cstdint>
#include <iostream>

namespace nes
{
    constexpr uint8_t BIT_0 = 0x01; // 0000 0001
    constexpr uint8_t BIT_1 = 0x02; // 0000 0010
    constexpr uint8_t BIT_2 = 0x04; // 0000 0100
    constexpr uint8_t BIT_3 = 0x08; // 0000 1000
    constexpr uint8_t BIT_4 = 0x10; // 0001 0000
    constexpr uint8_t BIT_5 = 0x20; // 0010 0000
    constexpr uint8_t BIT_6 = 0x40; // 0100 0000
    constexpr uint8_t BIT_7 = 0x80; // 1000 0000

    constexpr uint8_t SIGN_BIT_INT8 = BIT_7;
    constexpr uint16_t CARRY_TEST = 0x00FF;

    bool isSigned(uint8_t val);
    bool testBitMask8Bit(uint8_t bit, uint8_t value);
    bool testBitMask16Bit(uint16_t bit, uint16_t value);
    bool isCarryFromUnsignedAdd(uint16_t value);
    bool isOverflowFromSignedAdd(uint8_t a, uint8_t b, uint8_t result);
}
