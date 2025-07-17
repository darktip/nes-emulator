#include "CPU6502.h"

void CPU6502::resetInternal()
{
    a_reg = 0x00;
    x_reg = 0x00;
    y_reg = 0x00;
    pc_reg = 0x0000;
    sp_reg = 0x00;
    status_reg = 0x00;

    fetch = 0x00;
    cycles = 0;
    cyclesCount = 0;
}

CPU6502::CPU6502()
{
    resetInternal();
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

void CPU6502::setStatusFlag(StatusRegistryFlags flag, bool predicate)
{
    if (predicate)
    {
        status_reg |= flag;
    }
    else
    {
        status_reg &= ~flag;
    }
}

uint8_t CPU6502::getStatusFlag(StatusRegistryFlags flag)
{
    return status_reg & flag;
}

