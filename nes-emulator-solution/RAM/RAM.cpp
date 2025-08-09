#include "RAM.h"

using namespace nes;

RAM::RAM()
{
    for (auto &t : ram)
    {
        t = 0x00;
    }
}

RAM::~RAM() = default;

void RAM::write(uint16_t addr, uint8_t val)
{
    if (isValidAddress(addr))
    {
        ram[addr] = val;
    }
}

uint8_t RAM::read(uint16_t addr) const
{
    if (isValidAddress(addr))
    {
        return ram[addr];
    }

    return 0x00;
}