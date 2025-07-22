#include "Bus.h"

Bus::Bus(BusDevice* cpu, std::vector<BusDevice>* connectedDevices)
{
    this->cpu = cpu;
    this->connectedDevices = connectedDevices;

    this->cpu->connect(this);
    for (auto &t : *this->connectedDevices)
    {
        t.connect(this);
    }
}

Bus::~Bus() = default;

void Bus::write(uint16_t addr, uint8_t val)
{
    if (this->isValidAddress(addr))
    {
        for (auto &t : *connectedDevices)
        {
            if (t.isValidAddress(addr))
            {
                t.write(addr, val);   
            }
        }
    }
}

uint8_t Bus::read(uint16_t addr) const
{
    if (this->isValidAddress(addr))
    {
        for (auto &t : *connectedDevices)
        {
            if (t.isValidAddress(addr))
            {
                return t.read(addr);   
            }
        }
    }

    return 0x00;
}

