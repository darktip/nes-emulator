#include "Bus.h"

bool Bus::isValidAddress(uint16_t addr)
{
    return addr >= 0x0000 && addr <= 0xFFFF;
}

Bus::Bus()
{
    cpu.connect(this);

    for (auto &t : ram)
    {
        t = 0x00;
    }
}

Bus::~Bus() = default;

void Bus::write(uint16_t addr, uint8_t val)
{
    if (isValidAddress(addr))
    {
        ram[addr] = val;
    }
}

uint8_t Bus::read(uint16_t addr)
{
    if (isValidAddress(addr))
    {
        return ram[addr];
    }

    return 0x00;
}

