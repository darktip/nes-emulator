#pragma once

#include "IODevice.h"

namespace nes
{
    class BusDevice : public IODevice
    {
    protected:
        IODevice* bus;
    public:
        bool isValidAddress(uint16_t addr) const override;
        void connect(IODevice* bus);
    };
}