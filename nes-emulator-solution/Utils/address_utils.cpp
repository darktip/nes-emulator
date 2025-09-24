#include "address_utils.h"

#include "../Bus/IODevice.h"

using namespace nes;

uint16_t nes::readFullAddressWithIncrementPC(const IODevice& io, uint16_t& programCounter)
{
    uint16_t result = readFullAddress(io, programCounter);
    programCounter += 2;
    return result;
}

uint16_t nes::readFullAddress(const IODevice& io, uint16_t address)
{
    uint16_t low_byte = io.read(address);
    uint16_t high_byte = io.read(address + 1);
    uint16_t result = static_cast<uint16_t>(high_byte << HI_BYTE_SHIFT) | low_byte;
    return result;
}

void nes::writeFullAddress(IODevice& io, uint16_t startWriteAddress, uint16_t address)
{
    uint8_t low_byte = address & LO_BYTE_MASK;
    uint8_t high_byte = (address & HI_BYTE_MASK) >> HI_BYTE_SHIFT;
    io.write(startWriteAddress, low_byte);
    io.write(startWriteAddress + 1, high_byte);
}

bool nes::isAddressOnSamePage(uint16_t a, uint16_t b)
{
    bool result = (a & HI_BYTE_MASK) == (b & HI_BYTE_MASK);
    return result;
}
