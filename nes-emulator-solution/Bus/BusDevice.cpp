#include "BusDevice.h"

bool BusDevice::isValidAddress(uint16_t addr)
{
    return addr >= 0x0000 && addr <= 0xFFFF;
}

void BusDevice::connect(IODevice* bus)
{
    this->bus = bus;
}
