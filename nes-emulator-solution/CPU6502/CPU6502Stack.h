#pragma once

#include <cstdint>
#include "../Bus/IODevice.h"

namespace nes
{
    class CPU6502Stack
    {
        static constexpr uint16_t STACK_END_ADDRESS = 0x01FF;
        static constexpr uint16_t STACK_BASE_ADDRESS = 0x0100;

    public:
        CPU6502Stack(IODevice* cpu, uint8_t* stackPointer);

        void push(uint8_t byte);
        uint8_t pop();

    private:
        IODevice* cpu;
        uint8_t* stackPointer;

        uint16_t getAddress();
    };
}
