#include "CPU6502.h"

void CPU6502::reset()
{
    a_reg = 0x00;
    x_reg = 0x00;
    y_reg = 0x00;
    pc_reg = 0x0000;
    sp_reg = 0x00;
    status_reg = 0x00;

    fetch = 0x00;
}

CPU6502::CPU6502()
{
    reset();
}

CPU6502::~CPU6502() = default;

void CPU6502::write(uint16_t addr, uint8_t val)
{
    bus->write(addr, val);
}

uint8_t CPU6502::read(uint16_t addr)
{
    return bus->read(addr);
}

