#include "address_utils.h"

#include "../Bus/IODevice.h"

uint16_t readFullAddressWithIncrementPC(const IODevice& io, uint16_t& programCounter)
{
    uint16_t low_byte = io.read(programCounter++);
    uint16_t high_byte = io.read(programCounter++);
    uint16_t result = static_cast<uint16_t>(high_byte << HI_BYTE_SHIFT) | low_byte;
    return result;
}

uint16_t readFullAddress(const IODevice& io, uint16_t address)
{
    uint16_t low_byte = io.read(address);
    uint16_t high_byte = io.read(address + 1);
    uint16_t result = static_cast<uint16_t>(high_byte << HI_BYTE_SHIFT) | low_byte;
    return result;
}

bool isAddressOnSamePage(uint16_t a, uint16_t b)
{
    bool result = (a & HI_BYTE_MASK) == (b & HI_BYTE_MASK);
    return result;
}
