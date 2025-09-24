#include "NES.h"

#include <sstream>

#include "../Utils/address_utils.h"

nes::NES::NES() : bus(&cpu)
{
    bus.ConnectDevice(&ram);
}

void nes::NES::LoadProgram(std::string program, uint16_t offset)
{
    std::stringstream ss;
    ss << program;

    uint16_t address = offset;
    while (!ss.eof())
    {
        std::string hex;
        ss >> hex;
        uint8_t value = static_cast<uint8_t>(std::stoul(hex, nullptr, 16));
        ram.write(address++, value); 
    }

    writeFullAddress(ram, CPU6502::SYSTEM_VECTOR_RES, offset);
    cpu.reset();
}

void nes::NES::Run()
{
    while (!cpu.clock())
    {
    }
}
