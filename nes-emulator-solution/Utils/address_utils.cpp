#include "address_utils.h"

#include "../Bus/IODevice.h"

using namespace nes;

uint16_t nes::readFullAddressWithIncrementPC(const IODevice& io, uint16_t& programCounter)
{
    uint16_t low_byte = io.read(programCounter++);
    uint16_t high_byte = io.read(programCounter++);
    uint16_t result = static_cast<uint16_t>(high_byte << HI_BYTE_SHIFT) | low_byte;
    return result;
}

uint16_t nes::readFullAddress(const IODevice& io, uint16_t address)
{
    uint16_t low_byte = io.read(address);
    uint16_t high_byte = io.read(address + 1);
    uint16_t result = static_cast<uint16_t>(high_byte << HI_BYTE_SHIFT) | low_byte;
    return result;
}

bool nes::isAddressOnSamePage(uint16_t a, uint16_t b)
{
    bool result = (a & HI_BYTE_MASK) == (b & HI_BYTE_MASK);
    return result;
}
