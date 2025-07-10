#pragma once
#include <array>
#include <cstdint>

#include "../CPU6502/CPU6502.h"

class Bus
{
private:
    std::array<uint8_t, 64 * 1024> ram; // 64KB of RAM for testing. Temporary entity
    CPU6502 cpu;

    bool isValidAddress(uint16_t addr);
public:
    Bus();
    ~Bus();
    
    void write(uint16_t addr, uint8_t val);
    uint8_t read(uint16_t addr);
};
