#pragma once
#include <string>

#include "../CPU6502/CPU6502.h"
#include "../Bus/Bus.h"
#include "../RAM/RAM.h"

namespace nes
{
    class NES
    {
    public:
        NES();

        void LoadProgram(std::string program, uint16_t offset);
        void Run();

    private:
        CPU6502 cpu;
        Bus bus;
        RAM ram;
    };
}
