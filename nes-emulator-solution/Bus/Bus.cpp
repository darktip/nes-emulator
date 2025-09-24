#include "Bus.h"

using namespace nes;

Bus::Bus(BusDevice* cpu)
{
    this->cpu = cpu;
    this->cpu->connect(this);
}

Bus::~Bus() = default;

void Bus::ConnectDevice(BusDevice* device)
{
    connectedDevices.push_back(device);
    device->connect(this);
}

void Bus::DisconnectDevice(BusDevice* device)
{
    std::erase(connectedDevices, device);
}

bool Bus::isValidAddress(uint16_t addr) const
{
    return addr >= 0x0000 && addr <= 0xFFFF;
}

void Bus::write(uint16_t addr, uint8_t val)
{
    if (this->isValidAddress(addr))
    {
        for (auto &t : connectedDevices)
        {
            if (t->isValidAddress(addr))
            {
                t->write(addr, val);   
            }
        }
    }
}

uint8_t Bus::read(uint16_t addr) const
{
    if (this->isValidAddress(addr))
    {
        for (auto &t : connectedDevices)
        {
            if (t->isValidAddress(addr))
            {
                return t->read(addr);   
            }
        }
    }

    return 0x00;
}

