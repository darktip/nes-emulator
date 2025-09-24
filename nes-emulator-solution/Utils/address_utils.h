#pragma once

#include <cstdint>

namespace nes
{
    class IODevice;
    
    constexpr uint16_t LO_BYTE_MASK = 0x00FF;
    constexpr uint16_t HI_BYTE_MASK = 0xFF00;
    constexpr uint16_t HI_BYTE_SHIFT = 8;

    uint16_t readFullAddressWithIncrementPC(const IODevice& io, uint16_t& programCounter);
    uint16_t readFullAddress(const IODevice& io, uint16_t address);
    void writeFullAddress(IODevice& io, uint16_t startWriteAddress, uint16_t address);
    bool isAddressOnSamePage(uint16_t a, uint16_t b);
}
