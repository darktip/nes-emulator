#pragma once
#include <cstdint>

class IODevice
{
public:
    virtual ~IODevice();
    
    virtual bool isValidAddress(uint16_t addr) const = 0;
    virtual void write(uint16_t addr, uint8_t val) = 0;
    virtual uint8_t read(uint16_t addr) const = 0;
};
