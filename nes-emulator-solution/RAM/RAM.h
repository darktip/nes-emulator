#pragma once

#include <array>
#include "../Bus/BusDevice.h"

namespace nes
{
    class RAM : public BusDevice
    {
    private:
        std::array<uint8_t, 64 * 1024> ram; // 64KB of RAM for testing. Temporary entity

    public:
        RAM();
        ~RAM() override;

        uint8_t read(uint16_t addr) const override;
        void write(uint16_t addr, uint8_t val) override;
    };
}
