#pragma once

#include <cstdint>

#include "../Bus/BusDevice.h"

class CPU6502 : public BusDevice
{
private:
    uint8_t a_reg = 0x00;
    uint8_t x_reg = 0x00;
    uint8_t y_reg = 0x00;
    uint16_t pc_reg = 0x0000;
    uint8_t sp_reg = 0x00;
    uint8_t status_reg = 0x00;

    uint8_t fetch = 0x00;
    
    void reset();
    
public:
    CPU6502();
    ~CPU6502();
    
    void write(uint16_t addr, uint8_t val);
    uint8_t read(uint16_t addr);
};
