#pragma once

#include <cstdint>
#include <vector>

#include "BusDevice.h"

namespace nes
{
    class Bus : IODevice
    {
        BusDevice* cpu;
        std::vector<BusDevice*> connectedDevices;

    public:
        Bus(BusDevice* cpu);
        ~Bus() override;

        void ConnectDevice(BusDevice* device);
        void DisconnectDevice(BusDevice* device);
        
        bool isValidAddress(uint16_t addr) const override;
        void write(uint16_t addr, uint8_t val) override;
        uint8_t read(uint16_t addr) const override;
    };
}
