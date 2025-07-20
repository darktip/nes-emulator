#pragma once
#include <cstdint>
#include <vector>

#include "BusDevice.h"

class Bus : IODevice
{
private:
    BusDevice* cpu;
    std::vector<BusDevice>* connectedDevices;

public:
    Bus(BusDevice* cpu, std::vector<BusDevice>* connectedDevices);
    ~Bus() override;
    
    void write(uint16_t addr, uint8_t val) override;
    uint8_t read(uint16_t addr) override;
};
